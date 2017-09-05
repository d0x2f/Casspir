#include <cassert>
#include <cstdlib>
#include <vector>

#include <casspir.hh>

static void test_mine_flip()
{
    std::vector<Casspir::Point> mines = {
        Casspir::Point(3,1),
        Casspir::Point(3,8),
        Casspir::Point(5,6),
        Casspir::Point(9,1),
        Casspir::Point(7,7),
        Casspir::Point(4,9),
        Casspir::Point(5,4),
        Casspir::Point(7,3),
        Casspir::Point(2,0),
        Casspir::Point(5,1)
    };
    Casspir::Map map = casspir_make_map(10,10, mines);

    //The number of tiles flipped should be 0.
    assert( map.get_num_flipped() == 0 );

    map.flip(Casspir::Point(6,9));
    map.flag(Casspir::Point(7,7));

    //The number of tiles flipped should be 28.
    assert( map.get_num_flipped() == 28 );

    map.flip(Casspir::Point(6,8));

    //The number of tiles flipped should be 30.
    assert( map.get_num_flipped() == 30 );

    //The game should be in progress
    assert( map.get_status() == Casspir::MapStatus::IN_PROGRESS );

    map.flip(Casspir::Point(2,0));

    //The game should be failed
    assert( map.get_status() == Casspir::MapStatus::FAILED );
}

int main (void)
{
    test_mine_flip();

    return EXIT_SUCCESS;
}
