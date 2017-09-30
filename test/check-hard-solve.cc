#include <cassert>
#include <cstdlib>
#include <set>

#include <casspir.hh>
#include <Solver.hh>

static void test_hard_solve()
{
    std::set<Casspir::Point> mines = {
        Casspir::Point(0,0),
        Casspir::Point(1,0),
        Casspir::Point(2,0),
        Casspir::Point(3,1),
        Casspir::Point(4,1),
        Casspir::Point(5,1),
        Casspir::Point(7,1),
        Casspir::Point(8,1),
        Casspir::Point(2,2),
        Casspir::Point(6,2),
        Casspir::Point(9,2),
        Casspir::Point(0,4),
        Casspir::Point(2,5),
        Casspir::Point(4,5),
        Casspir::Point(5,5),
        Casspir::Point(8,5),
        Casspir::Point(9,5),
        Casspir::Point(1,6),
        Casspir::Point(9,6),
        Casspir::Point(0,7),
        Casspir::Point(1,7),
        Casspir::Point(4,7),
        Casspir::Point(4,8),
        Casspir::Point(3,9),
        Casspir::Point(8,9)
    };
    Casspir::Map map = casspir_make_map(10,10, mines);

    //The number of tiles flipped should be 0.
    assert( map.get_num_flipped() == 0 );

    map.flip(Casspir::Point(6,9));

    casspir_solve(map);

    //Map should be completed
    assert( map.get_status() == Casspir::MapStatus::COMPLETE );

    //75 tiles should be flipped
    assert( map.get_num_flipped() == 75 );

    //0 mines should remain
    assert( map.get_mines_remaining() == 0 );

    map.reset();

    //Map should be in progress
    assert( map.get_status() == Casspir::MapStatus::IN_PROGRESS );

    //0 tiles should be flipped
    assert( map.get_num_flipped() == 0 );

    //20 mines should remain
    assert( map.get_mines_remaining() == 25 );
}

int main (void)
{
    test_hard_solve();

    return EXIT_SUCCESS;
}
