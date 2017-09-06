#include <cassert>
#include <cstdlib>

#include <casspir.hh>

static void test_generate_small_puzzle()
{
    Casspir::Map map = casspir_generate_map(10,10, 100, Casspir::Point(5,6));

    //Get the map state
    std::vector<Casspir::TileState> state = map.get_state();

    //Check dimensions
    assert( map.get_width() == 10 );
    assert( map.get_height() == 10 );

    //The map should have 100 tiles.
    assert( state.size() == 100 );

    //The map should have at least 5 mines.
    assert( map.get_mines_remaining() >= 5 );
}

int main (void)
{
    test_generate_small_puzzle();

    return EXIT_SUCCESS;
}
