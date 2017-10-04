#include <random>
#include <iostream>
#include <cassert>

#include "Map.hh"

using namespace Casspir;

/**
 * Initialise a width*height minesweeper map with randomly placed mines.
 *
 * @param width Width
 * @param height Height
 * @param difficulty Difficulty factor 0-255
 * @param first_flip Coordinate of the players first move.
 */
Map::Map(uint32_t width, uint32_t height, uint8_t difficulty, Point first_flip)
 : Map(width, height)
 {
    std::random_device r_device;
    std::default_random_engine r_engine(r_device());
    std::uniform_real_distribution<> r_distribution(0, 1);

    //Get first_flip neighbourhood so as not to place mines in there.
    std::set<Point> first_flip_neighbourhood = this->get_neighbours(first_flip);
    first_flip_neighbourhood.insert(first_flip);

    //Place mines randomly
    this->total_mines = 0;
    float mine_probability = ((float)(difficulty+20)) / 512.f;
    for (size_t i = 0; i < this->state.size(); i++) {
        Point position = Point::from_index(i, this->width);

        //Place if random value breaks difficulty threshold
        //But not if this is the first flipped tile
        if (r_distribution(r_engine) < mine_probability && first_flip_neighbourhood.count(position) == 0) {
            this->state[i].mine = true;
            this->total_mines += 1;

            //Increment neighbour values
            for (const auto& neighbour : this->get_neighbours(position)) {
                this->get_tile(neighbour).value ++;
            }
        }
    }

    this->mines_remaining = this->total_mines;

    //Flip the first tile
    this->flip_recurse(first_flip);
}

/**
 * Initialise a width*height minesweeper map with mines in the given positions.
 *
 * @param width Width
 * @param height Height
 * @param mines A list of mine positions.
 */
Map::Map(uint32_t width, uint32_t height, std::set<Point> mines)
: Map(width, height)
{
    for(auto& mine : mines) {
        this->get_tile(mine).mine = true;

        //Increment neighbour values
        for (const auto& neighbour : this->get_neighbours(mine)) {
            this->get_tile(neighbour).value ++;
        }
    }

    this->total_mines = mines.size();
    this->mines_remaining = this->total_mines;
}

/**
 * Initialise a width*height minesweeper map.
 *
 * @param width Width
 * @param height Height
 */
Map::Map(uint32_t width, uint32_t height)
    : width(width), height(height)
{
    this->state.resize(width*height);
    this->status = MapStatus::IN_PROGRESS;
    this->tiles_flipped = 0;
}

/**
 * If the tile is unflipped, flip it and adjoining tiles recusively while tile value is non zero.
 * If the tile is flipped, expand adjoining unflipped tiles.
 *
 * @param position
 *
 * @return The number of tiles flipped.
 */
uint64_t Map::flip(Point position)
{
    TileState& tile = this->get_tile(position);
    std::set<Point> neighbours = this->get_neighbours(position);
    uint64_t flipped = 0;

    if (tile.flipped) {
        //Already flipped,
        //check if it's number is satisfied by flags and flip the neighbours.
        if (this->is_tile_satisfied(position)) {
            for (const auto& neighbour : neighbours) {
                flipped += this->flip_recurse(neighbour);
            }
        }
    } else if (!tile.flagged) {
        //Not yet flipped or flagged, let the flippening begin.
        flipped = this->flip_recurse(position);
    }

    this->check_completed();

    return flipped;
}

/**
 * Flag the given position as a mine
 * This position won't be flipped automatically.
 *
 * @param position
 */
void Map::flag(Point position)
{
    if (this->get_status() != MapStatus::IN_PROGRESS) {
        return;
    }

    TileState& tile = this->get_tile(position);
    if (!tile.flipped) {
        if (tile.flagged) {
            tile.flagged = false;
            this->mines_remaining += 1;
        } else {
            //Only allow if there are any mines remaining
            if (this->mines_remaining > 0) {
                tile.flagged = true;
                this->mines_remaining -= 1;
            }
        }
    }

    this->check_completed();
}

/**
 * Checks whether the game has been completed.
 */
void Map::check_completed()
{
    if (this->status != MapStatus::IN_PROGRESS) {
        return;
    }

    if (this->get_mines_remaining() == 0
    && (this->get_num_flipped() + this->get_total_mines()) == this->state.size()
    ) {
        this->status = MapStatus::COMPLETE;
    }
}

/**
 * Reset the puzzle to the initial state.
 * The fist flip will be undone, so the next
 * flip might be a mine.
 */
void Map::reset()
{
    for (auto& tile : this->state) {
        tile.flipped = false;
        tile.flagged = false;
    }
    this->mines_remaining = this->total_mines;
    this->tiles_flipped = 0;
    this->status = MapStatus::IN_PROGRESS;
}

/**
 * Recurively flip this tile and it's neighbours.
 * Continue recursion if tile value is zero.
 *
 * @param position The position to flip and recurse from
 *
 * @return The number of tiles flipped.
 */
uint64_t Map::flip_recurse(Point position)
{
    if (this->get_status() != MapStatus::IN_PROGRESS) {
        return 0;
    }

    TileState& tile = this->get_tile(position);

    //If the tile is already flipped or flagged, ignore it.
    if (tile.flipped || tile.flagged) {
        return 0;
    }

    //Flip the tile.
    tile.flipped = true;
    this->tiles_flipped++;

    //If the tile is a mine, fail the game
    if (tile.mine) {
        this->status = MapStatus::FAILED;
        return 1;
    }

    //If the tile value is non-zero we're done.
    if (tile.value != 0) {
        return 1;
    }

    //Recurse into the neighbours.
    std::set<Point> neighbours = this->get_neighbours(position);

    uint64_t flipped = 1;
    for (const auto neighbour : neighbours) {
        flipped += this->flip_recurse(neighbour);
    }
    return flipped;
}

/**
 * Get the map width.
 *
 * @return width
 */
uint32_t Map::get_width()
{
    return this->width;
}

/**
 * Get the map height.
 *
 * @return height
 */
uint32_t Map::get_height()
{
    return this->height;
}

/**
 * Get the map state.
 *
 * @return state
 */
std::vector<TileState>& Map::get_state()
{
    return this->state;
}

/**
 * Get the number of flipped tiles.
 *
 * @return number of flipped tiles
 */
uint64_t Map::get_num_flipped()
{
    return this->tiles_flipped;
}

/**
 * Get the number of mines left.
 *
 * @return mines remaining
 */
uint64_t Map::get_mines_remaining()
{
    return this->mines_remaining;
}

/**
 * Get the number of mines in this puzzle.
 *
 * @return Number of mines on the map.
 */
uint64_t Map::get_total_mines()
{
    return this->total_mines;
}

/**
 * Get the current map status.
 *
 * @return The map status
 */
MapStatus Map::get_status()
{
    return this->status;
}

/**
 * Get the tile in the given position.
 *
 * @return A tile
 */
TileState& Map::get_tile(Point position)
{
    uint64_t index = position.get_index(this->width);
    return this->get_tile(index);
}

/**
 * Get the tile in the given position.
 *
 * @return A tile
 */
TileState& Map::get_tile(uint64_t index)
{
    assert (index < this->state.size());
    return this->state.at(index);
}

bool Map::is_tile_satisfied(Point position)
{
    TileState& tile = this->get_tile(position);
    std::set<Point> neighbours = this->get_neighbours(position);
    uint8_t flags = 0;
    for (const auto& neighbour : neighbours) {
        flags += this->get_tile(neighbour).flagged;
    }
    return (flags == tile.value);
}

/**
 * Print the board to stdout
 */
void Map::print(bool revealed)
{
    for (size_t i = 0; i < this->state.size(); i++) {
        if ((i % this->width) == 0) {
            std::cout << std::endl;
        }
        char token;
        TileState tile = this->state[i];
        if (tile.flipped || revealed) {
            if (tile.mine) {
                token = '*';
            } else {
                token = '0' + tile.value;
            }
        } else if (tile.flagged) {
            token = '^';
        } else {
            token = '#';
        }
        std::cout << token;
    }
    std::cout << std::endl;
}

/**
 * Find the neighbour positions of a tile.
 *
 * @param position The position to find neighbours for.
 */
std::set<Point> Map::get_neighbours(Point position)
{
    std::set<Point> neighbours;

    bool \
        U = position.y > 0,
        D = position.y < (this->height - 1),
        L = position.x > 0,
        R = position.x < (this->width - 1);

    if (U) {
        neighbours.insert(Point(position.x, position.y-1));
    }

    if (D) {
        neighbours.insert(Point(position.x, position.y+1));
    }

    if (L) {
        neighbours.insert(Point(position.x-1, position.y));
    }

    if (R) {
        neighbours.insert(Point(position.x+1, position.y));
    }

    if (U && L) {
        neighbours.insert(Point(position.x-1, position.y-1));
    }

    if (U && R) {
        neighbours.insert(Point(position.x+1, position.y-1));
    }

    if (D && L) {
        neighbours.insert(Point(position.x-1, position.y+1));
    }

    if (D && R) {
        neighbours.insert(Point(position.x+1, position.y+1));
    }

    return neighbours;
}