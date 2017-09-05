#include <random>
#include <iostream>

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

    //Place mines randomly
    this->mines_remaining = 0;
    float mine_probability = ((float)(difficulty+20)) / 512.f;
    for (size_t i = 0; i < this->state.size(); i++) {
        Point position = Point::from_index(i, this->width);

        //Place if random value breaks difficulty threshold
        //But not if this is the first flipped tile
        if (r_distribution(r_engine) < mine_probability && position != first_flip) {
            this->state[i].mine = true;
            this->mines_remaining += 1;

            //Increment neighbour values
            for (const auto& neighbour : this->get_neighbours(position)) {
                this->state[neighbour.get_index(this->width)].value ++;
            }
        }
    }

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
Map::Map(uint32_t width, uint32_t height, std::vector<Casspir::Point> mines)
: Map(width, height)
{
    for(auto& mine : mines) {
        this->state[mine.get_index(width)].mine = true;
        this->mines_remaining += 1;

        //Increment neighbour values
        for (const auto& neighbour : this->get_neighbours(mine)) {
            this->state[neighbour.get_index(this->width)].value ++;
        }
    }

    this->mines_remaining = mines.size();
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
 */
void Map::flip(Point position)
{
    TileState tile = this->state[position.get_index(this->width)];
    std::vector<Point> neighbours = this->get_neighbours(position);

    if (tile.flipped) {
        //Already flipped,
        //check if it's number is satisfied by flags and flip the neighbours.
        uint8_t flags = 0;
        for (const auto& neighbour : neighbours) {
            flags += this->state[neighbour.get_index(this->width)].flagged;
        }
        if (flags >= tile.value) {
            for (const auto& neighbour : neighbours) {
                TileState neighbour_tile = this->state[neighbour.get_index(this->width)];
                if (!(neighbour_tile.flipped || tile.flagged)) {
                    this->flip_recurse(neighbour);
                }
            }
        }
    } else if (!tile.flagged) {
        //Not yet flipped or flagged, let the flippening begin.
        this->flip_recurse(position);
    }
}

/**
 * Flag the given position as a mine
 * This position won't be flipped automatically.
 *
 * @param position
 */
void Map::flag(Point position)
{
    uint64_t index = position.get_index(this->width);
    if (!this->state[index].flipped) {
        this->state[index].flagged ^= true;
    }
}

/**
 * Recurively flip this tile and it's neighbours.
 * Continue recursion if tile value is zero.
 *
 * @param position The position to flip and recurse from
 */
void Map::flip_recurse(Point position)
{
    TileState tile = this->state[position.get_index(this->width)];

    //If the tile is already flipped or flagged, ignore it.
    if (tile.flipped || tile.flagged) {
        return;
    }

    //Flip the tile.
    this->state[position.get_index(this->width)].flipped = true;
    this->tiles_flipped++;

    //If the tile is a mine, fail the game
    if (tile.mine) {
        this->status = MapStatus::FAILED;
        return;
    }

    //If the tile value is non-zero we're done.
    if (tile.value != 0) {
        return;
    }

    //Recurse into the neighbours.
    std::vector<Point> neighbours = this->get_neighbours(position);

    for (const auto neighbour : neighbours) {
        this->flip_recurse(neighbour);
    }
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
std::vector<TileState> Map::get_state()
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
 * Get the current map status.
 *
 * @return The map status
 */
MapStatus Map::get_status()
{
    return this->status;
}

/**
 * Print the board to stdout
 */
void Map::print()
{
    for (size_t i = 0; i < this->state.size(); i++) {
        if ((i % this->width) == 0) {
            std::cout << std::endl;
        }
        char token;
        TileState tile = this->state[i];
        if (tile.flipped) {
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
std::vector<Point> Map::get_neighbours(Point position)
{
    std::vector<Point> neighbours;

    bool \
        U = position.y > 0,
        D = position.y < (this->height - 1),
        L = position.x > 0,
        R = position.x < (this->width - 1);

    if (U) {
        neighbours.push_back(Point(position.x, position.y-1));
    }

    if (D) {
        neighbours.push_back(Point(position.x, position.y+1));
    }

    if (L) {
        neighbours.push_back(Point(position.x-1, position.y));
    }

    if (R) {
        neighbours.push_back(Point(position.x+1, position.y));
    }

    if (U && L) {
        neighbours.push_back(Point(position.x-1, position.y-1));
    }

    if (U && R) {
        neighbours.push_back(Point(position.x+1, position.y-1));
    }

    if (D && L) {
        neighbours.push_back(Point(position.x-1, position.y+1));
    }

    if (D && R) {
        neighbours.push_back(Point(position.x+1, position.y+1));
    }

    return neighbours;
}