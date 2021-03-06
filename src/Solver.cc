#include <iostream>
#include <map>
#include <random>

#include "Solver.hh"
#include "definitions.hh"

using namespace Casspir;

Solver::Solver(Map& map) : map(map)
{
    this->map_size = this->map.get_width() * this->map.get_height();

    this->random_engine.seed(41418740515);
    this->random_int = std::uniform_int_distribution<uint64_t>(
        0,
        this->map_size
    );
}

std::queue<Operation> Solver::solve()
{
    bool did_something = false;
    do {
        //Try basic
        if (!this->perform_basic_pass()) {
            //Try permutation
            if (!this->enumerate_groups()) {
                //Do random
                this->flip_random_tile();
            }
        }
    } while (this->map.get_status() == MapStatus::IN_PROGRESS);

    return this->operations;
}

void Solver::flip_random_tile()
{
    uint64_t random_index = this->random_int(this->random_engine) % (this->map_size - this->map.get_num_flipped());

    uint64_t index = 0;
    for (uint64_t i=0; i < this->map_size; i++) {
        TileState tile = this->map.get_tile(i);
        if (!tile.flipped) {
            if (index == random_index) {
                this->flip(Point::from_index(i, this->map.get_width()));
                return;
            }
            index++;
        }
    }
}

/**
 * Loop over each tile and evaluate each once.
 *
 * @return true if an action was performed.
 */
bool Solver::perform_basic_pass()
{
    bool did_something = false;

    for (uint64_t i=0; i < this->map_size; i++) {
        TileState tile = this->map.get_tile(i);
        if (tile.flipped && tile.value > 0) {
            did_something |= this->evaluate_neighbours(i);

            if (this->map.get_status() != MapStatus::IN_PROGRESS) {
                break;
            }
        }
    }

    return did_something;
}

/**
 * Evalue the neighbours of the given tile and see if anything can be deduced.
 *
 * @param index The tile index to consider.
 *
 * @return true if an action was performed.
 */
bool Solver::evaluate_neighbours(uint64_t index)
{
    bool did_something = false;
    TileState tile = this->map.get_tile(index);
    Point tile_position = Point::from_index(index, this->map.get_width());

    std::set<Point> neighbours = this->map.get_neighbours(tile_position);

    //Count the flagged and unflipped neighbours
    uint8_t flagged = 0;
    uint8_t unflipped = 0;
    for (const auto& neighbour : neighbours) {
        TileState neighbour_tile = this->map.get_tile(neighbour);
        flagged += (neighbour_tile.flagged);
        unflipped += (!neighbour_tile.flipped);
    }

    if (flagged == tile.value && unflipped - flagged > 0) {
        //If this tile's values is satisfied by flagged neighbours, flip the rest
        did_something |= this->flip(tile_position);
    } else if (unflipped == tile.value) {
        //Otherwise if the number of unflipped match the tiles value, then flag the unflipped.
        for (const auto& neighbour : neighbours) {
            TileState neighbour_tile = this->map.get_tile(neighbour);
            if (!neighbour_tile.flagged && !neighbour_tile.flipped) {
                did_something |= this->flag(neighbour);
            }
        }
    }

    return did_something;
}

/**
 * Find all groups of tiles.
 *
 * @return Whether a move was taken.
 */
bool Solver::enumerate_groups()
{
    TileState tile;
    Point tile_position;
    std::set<Point> neighbours;
    std::set<Point> considered;
    std::set< std::pair< Point, float> > candidates;

    //If the number of remaining tiles is less than 20,
    //just evaluate all of them.
    if (this->map_size - this->map.get_num_flipped() < 20) {
        std::set<Point> border_unflipped;
        std::set<Point> border_flipped;

        for (uint64_t i=0; i < this->map_size; i++) {
            tile = this->map.get_tile(i);
            tile_position = Point::from_index(i, this->map.get_width());
            if (!tile.flipped) {
                border_unflipped.insert(tile_position);
                neighbours = this->map.get_neighbours(tile_position);

                for (Point neighbour : neighbours) {
                    TileState neighbour_tile = this->map.get_tile(neighbour);
                    if (neighbour_tile.flipped) {
                        border_flipped.insert(neighbour);
                    }
                }
            }
        }

        candidates = this->evaluate_group(border_unflipped, border_flipped);
    } else {
        //Loop over each tile and consider it's group.
        for (uint64_t i=0; i < this->map_size; i++) {
            tile = this->map.get_tile(i);
            if (tile.flipped) {
                continue;
            }

            tile_position = Point::from_index(i, this->map.get_width());

            if (considered.count(tile_position) > 0) {
                continue;
            }

            std::set<Point> border_unflipped;
            std::set<Point> border_flipped;

            this->recursive_border_search(tile_position, border_unflipped, border_flipped);
            considered.insert(border_unflipped.begin(), border_unflipped.end());

            if (border_unflipped.size() > 0 && border_unflipped.size() < 20) {
                std::set< std::pair< Point, float > > nominations = this->evaluate_group(
                    border_unflipped,
                    border_flipped
                );
                candidates.insert(nominations.begin(), nominations.end());
            }
        }
    }

    float min_risk = 1.;
    Point min_risk_point;
    bool min_risk_point_found = false;
    bool zero_risk_flip = false;
    for (const auto& candidate : candidates) {
        if (candidate.second == 0) {
            this->flip(candidate.first);
            zero_risk_flip = true;
            continue;
        }

        if (!min_risk_point_found || candidate.second < min_risk) {
            min_risk = candidate.second;
            min_risk_point = candidate.first;
            min_risk_point_found = true;
        }
    }

    return zero_risk_flip || (min_risk_point_found && this->flip(min_risk_point));
}

std::set< std::pair< Point, float > > Solver::evaluate_group(
    const std::set<Point> border_unflipped,
    const std::set<Point> border_flipped
) {
    Map staging_map = this->map;
    uint64_t max_mines = std::min(this->map.get_mines_remaining(), border_unflipped.size());
    uint64_t mines = 0;
    uint64_t max = 1 << border_unflipped.size();
    std::map<Point, uint64_t> tallies;
    uint64_t total_valid_permutations = 0;

    for (Point position : border_unflipped) {
        tallies.insert(std::pair<Point, uint64_t>(position, 0));
    }

    for (uint64_t i = 0; i < max; i++) {
        mines = 0;
        uint64_t j = 0;
        for (Point position : border_unflipped) {
            if (i & (1 << j)) {
                //set flag
                staging_map.get_tile(position).flagged = true;

                //Skip if too many mines are used
                if (++mines > max_mines) {
                    //mfw
                    goto double_break;
                }
            } else {
                //unset flag
                staging_map.get_tile(position).flagged = false;
            }
            j++;
        }

        //check if flipped tiles are satisfied
        for (Point position : border_flipped) {
            //If not, move onto the next number
            if (!staging_map.is_tile_satisfied(position)) {
                goto double_break;
            }
        }
        total_valid_permutations++;

        //if they are add a point to the tiles tally if it has a flag on it
        for (Point position : border_unflipped) {
            if (staging_map.get_tile(position).flagged) {
                tallies[position]++;
            }
        }

        double_break:;
    }

    //Flag those that always had a flag when satisfied.
    //Flip those that never had a flag when satisfied.
    Point min_point;
    uint64_t min_value = total_valid_permutations+1;
    std::set< std::pair< Point, float > > nominations;
    for (auto& kv : tallies) {
        if (kv.second == 0) {
            nominations.insert(std::pair<Point, float>(kv.first, static_cast<float>(kv.second)/total_valid_permutations));
        } else if (kv.second == total_valid_permutations) {
            this->flag(kv.first);
        } else if (kv.second < min_value) {
            min_value = kv.second;
            min_point = kv.first;
        }
    }

    nominations.insert(std::pair<Point, float>(min_point, static_cast<float>(min_value)/total_valid_permutations));

    return nominations;
}

/**
 * Flip the given position and record the operation in the solution.
 *
 * @param position the position to flip.
 *
 * @return true if an action was performed.
 */
bool Solver::flip(Point position)
{
    //Add the operation to the solution only if anything was actually flipped.
    if (this->map.flip(position) > 0) {
        this->operations.push(Operation(OperationType::FLIP, position));
        return true;
    }
    return false;
}

/**
 * Flag the given position and record the operation in the solution.
 *
 * @param position the position to flag.
 *
 * @return true if an action was performed.
 */
bool Solver::flag(Point position)
{
    this->map.flag(position);
    this->operations.push(Operation(OperationType::FLAG, position));
    return true;
}

/**
 * Recursively search through the game space looking for a contiguous set of unflipped border tiles.
 * Border tiles being those that have a flipped tile as a neighbour.
 *
 * @param position The starting position
 * @param border_unflipped A set to fill with unflipped tiles that make up the group.
 * @param border_flipped A set to fill with flipped neighbours of the unflipped tiles.
 */
void Solver::recursive_border_search(
    Point position,
    std::set<Point>& border_unflipped,
    std::set<Point>& border_flipped
) {
    //Check if the tile is flipped, meaning it's not a border tile.
    TileState tile = this->map.get_tile(position);
    if (tile.flipped || tile.flagged) {
        return;
    }

    //Check if this tile has already been considered during the search
    if (border_unflipped.count(position) > 0) {
        return;
    }

    //Search the neighbors for a flipped tile, confirming that this is a border tile.
    bool is_border_tile = false;
    std::set<Point> neighbours = this->map.get_neighbours(position);
    std::set<Point> flipped_neighbours;
    for (const auto& neighbour : neighbours) {
        TileState neighbour_tile = this->map.get_tile(neighbour);

        //Border tile confirmed
        if (!is_border_tile) {
            if (neighbour_tile.flipped) {
                border_unflipped.insert(position);
                is_border_tile = true;
            }
        }

        //Keep track of which neighbours are flipped.
        if (neighbour_tile.flipped) {
            border_flipped.insert(neighbour);
            flipped_neighbours.insert(neighbour);
        }
    }

    if (is_border_tile) {
        //Recurse into the border tiles neighbours to find the rest of the border
        for (const auto& neighbour : flipped_neighbours) {

            //Recurse into the unflipped neighbours of the flipped ones
            std::set<Point> candidate_neighbours = this->map.get_neighbours(neighbour);
            for (const auto& candidate_neighbour : candidate_neighbours) {
                this->recursive_border_search(candidate_neighbour, border_unflipped, border_flipped);
            }

        }
    }

    return;
}