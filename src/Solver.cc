#include <vector>

#include "Solver.hh"

using namespace Casspir;

Solver::Solver(Map& map) : map(map)
{
}

std::queue<Operation> Solver::solve()
{
    while (this->perform_pass()) {
        // :D
    }

    return this->operations;
}

/**
 * Loop over each tile and evaluate each once.
 *
 * @return true if an action was performed.
 */
bool Solver::perform_pass()
{
    bool did_something = false;
    uint64_t size = this->map.get_width() * this->map.get_height();

    for (uint64_t i=0; i < size; i++) {
        TileState tile = this->map.get_tile(i);
        if (tile.flipped && tile.value > 0) {
            did_something |= this->evaluate_neighbours(i);
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

    std::vector<Point> neighbours = this->map.get_neighbours(tile_position);

    //Count the flagged and unflipped neighbours
    uint8_t flagged = 0;
    uint8_t unflipped = 0;
    for (const auto& neighbour : neighbours) {
        TileState neighbour_tile = this->map.get_tile(neighbour);
        flagged += (neighbour_tile.flagged);
        unflipped += (!neighbour_tile.flipped);
    }

    if (flagged == tile.value && unflipped > 0) {
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
