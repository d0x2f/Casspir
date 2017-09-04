#include "casspir.hh"
#include "Map.hh"

/**
 * Generate a w*h minesweeper map.
 *
 * @param w Width
 * @param h Height
 * @param difficulty Difficulty factor 0-255
 * @param click Coordinate of the players first move.
 *
 * @return A new minesweeper map
 */
Casspir::Map casspir_generate_map(uint32_t w, uint32_t h, uint8_t difficulty, Casspir::Point click)
{
    return Casspir::Map(w, h, difficulty, click);
}

/**
 * Make a w*h minesweeper map with the mines in the given positions.
 *
 * @param w Width
 * @param h Height
 * @param mines A list of mine positions.
 *
 * @return A new minesweeper map
 */
Casspir::Map casspir_make_map(uint32_t w, uint32_t h, std::vector<Casspir::Point> mines)
{
    return Casspir::Map(w, h, mines);
}

/**
 * If the tile is unflipped, flip it and adjoining tiles recusively while tile value is non zero.
 * If the tile is flipped, expand adjoining unflipped tiles.
 *
 * @param map The game map.
 * @param position The posiiton to flip.
 *
 * @return The new map state after the flip.
 */
Casspir::Map casspir_evaluate_flip(Casspir::Map map, Casspir::Point position)
{
    map.flip(position);
}

std::vector<Casspir::Point> casspir_solve(Casspir::Map map)
{

}