#include "casspir.hh"
#include "Solver.hh"

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
 * Solve the given map.
 *
 * @param map The game map to solve.
 *
 * @return A list of tile operations in sequence.
 */
std::queue<Casspir::Operation> casspir_solve(Casspir::Map& map)
{
    Casspir::Solver solver(map);
    return solver.solve();
}

/**
 * I found this stub neccessary to satisfy an AC_CHECK_LIB macro in autotools.
 */
extern "C" int casspir_c_stub() {}