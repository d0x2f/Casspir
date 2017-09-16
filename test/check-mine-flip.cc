#include <cassert>
#include <cstdlib>
#include <vector>

#include <casspir.hh>

static void test_mine_flip()
{
    std::vector<Casspir::Point> mines = {
        Casspir::Point(3,1),
        Casspir::Point(6,0),
        Casspir::Point(8,3),
        Casspir::Point(4,4),
        Casspir::Point(6,9),
        Casspir::Point(1,7),
        Casspir::Point(8,0),
        Casspir::Point(7,3),
        Casspir::Point(3,6),
        Casspir::Point(5,4)
    };
    Casspir::Map map = casspir_make_map(10,10, mines);

    //The number of tiles flipped should be 0.
    assert( map.get_num_flipped() == 0 );

    //Should have flipped 25 tiles
    assert( map.flip(Casspir::Point(0,2)) == 25 );

    //Should have flipped 43 tiles
    assert( map.flip(Casspir::Point(9,9)) == 43 );

    //The number of tiles flipped should be 68.
    assert( map.get_num_flipped() == 68 );

    //The game should be in progress
    assert( map.get_status() == Casspir::MapStatus::IN_PROGRESS );

    map.flip(Casspir::Point(8,0));

    //The game should be failed
    assert( map.get_status() == Casspir::MapStatus::FAILED );
}

int main (void)
{
    test_mine_flip();

    return EXIT_SUCCESS;
}
