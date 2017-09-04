#include <cassert>
#include <cstdlib>

#include <casspir.hh>

static void test_first_tiles_flipped()
{
    Casspir::Map map = casspir_generate_map(10,10, 10, Casspir::Point(2,9));

    //The number of tiles flipped should be greater than 0.
    assert( map.get_num_flipped() > 0 );

    //The game should not be failed
    assert( map.get_status() != Casspir::MapStatus::FAILED );
}

int main (void)
{
    test_first_tiles_flipped();

    return EXIT_SUCCESS;
}
