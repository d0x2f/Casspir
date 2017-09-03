#include <cassert>
#include <cstdlib>
#include <vector>
#include <iostream>

#include <casspir.hh>

static void test_generate_small_puzzle()
{
    Casspir::Map map = casspir_generate_map(10,10, 100, Casspir::Point(5,6));

    //Get the map state
    std::vector<Casspir::TileState> state = map.get_state();

    //The map should have 100 tiles.
    assert( state.size() == 100 );

    //The map should have at least 5 mines.
    int mines = 0;
    for (const auto& tile : state) {
        mines += (tile.value == Casspir::TileValue::MINE);
    }

    assert( mines >= 5 );
}

int main (void)
{
    test_generate_small_puzzle();

    return EXIT_SUCCESS;
}
