
#include "casspir.hh"

/*
 * Generate an n*m minesweeper map.
 *
 * @param w Width
 * @param h Height
 * @param difficulty Dificulty factor 0-255
 *
 * @return A w*x size vector of tiles.
 */
std::vector<Casspir::TileValues> generate_map(uint32_t w, uint32_t h, uint8_t difficulty)
{
    return std::vector<Casspir::TileValues>(w*h);
}