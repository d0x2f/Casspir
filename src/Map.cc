#include <random>

#include "Map.hh"

using namespace Casspir;

/**
 * Initialise a width*height minesweeper map.
 *
 * @param width Width
 * @param height Height
 * @param difficulty Difficulty factor 0-255
 * @param click Coordinate of the players first move.
 *
 * @return A width*height size vector of tiles.
 */
Map::Map(uint32_t width, uint32_t height, uint8_t difficulty, Point click)
    : width(width), height(height), difficulty(difficulty)
{
    std::random_device r_device;
    std::default_random_engine r_engine(r_device());
    std::uniform_real_distribution<> r_distribution(0, 1);

    this->state.resize(width*height);

    if (difficulty == 0) {
        difficulty++;
    }

    //Place mines
    float mine_probability = ((float)(difficulty+100)) / 512.f;
    for (auto& tile : this->state) {
        if (r_distribution(r_engine) < mine_probability) {
            tile.value = TileValue::MINE;
        }
    }

    //Calculate remaining tile values
    //TODO
}

/**
 * If the tile is unflipped, flip it and adjoining tiles recusively while tile value is non zero.
 * If the tile is flipped, expand adjoining unflipped tiles.
 *
 * @param width Width
 */
void Map::flip(Point position)
{
    TileState tile = this->state[position.get_index()];

    std::vector<Point> neighbours = this->get_neighbours(position);

    if (tile.flipped) {
        //Already flipped, so flip the neighbours
        for (const auto& neighbour : neighbours) {
            this->flip(neighbour);
        }
    } else {
        //Not yet flipped, let the flippening begin.
        this->flip_recurse(position);
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
    TileState tile = this->state[position.get_index()];

    //If the tile is already flipped, ignore it.
    if (tile.flipped) {
        return;
    }

   //Flip the tile.
    this->state[position.get_index()].flipped = true;

    //If the tile value is non-zero we're done.
    if (tile.value == TileValue::ZERO && !tile.flipped) {
        return;
    }

    //Recurse into the neighbours.
    std::vector<Point> neighbours = this->get_neighbours(position);

    for (const auto& neighbour : neighbours) {
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