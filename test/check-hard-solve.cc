#include <cassert>
#include <cstdlib>
#include <set>

#include <casspir.hh>
#include <Solver.hh>

static void test_easy_solve()
{
    std::set<Casspir::Point> mines = {
        Casspir::Point(5,1),
        Casspir::Point(6,3),
        Casspir::Point(1,4),
        Casspir::Point(8,7),
        Casspir::Point(8,9),
        Casspir::Point(2,4),
        Casspir::Point(6,4),
        Casspir::Point(8,3),
        Casspir::Point(2,8),
        Casspir::Point(0,1),
        Casspir::Point(4,5),
        Casspir::Point(5,2),
        Casspir::Point(6,6),
        Casspir::Point(2,9),
        Casspir::Point(7,6),
        Casspir::Point(9,1),
        Casspir::Point(1,2),
        Casspir::Point(5,3),
        Casspir::Point(0,7),
        Casspir::Point(1,9)
    };
    Casspir::Map map = casspir_make_map(10,10, mines);

    //The number of tiles flipped should be 0.
    assert( map.get_num_flipped() == 0 );

    map.flip(Casspir::Point(6,9));

    casspir_solve(map);

    //Map should be completed
    assert( map.get_status() == Casspir::MapStatus::COMPLETE );

    //80 tiles should be flipped
    assert( map.get_num_flipped() == 80 );

    //0 mines should remain
    assert( map.get_mines_remaining() == 0 );

    map.reset();

    //Map should be in progress
    assert( map.get_status() == Casspir::MapStatus::IN_PROGRESS );

    //0 tiles should be flipped
    assert( map.get_num_flipped() == 0 );

    //20 mines should remain
    assert( map.get_mines_remaining() == 20 );
}

int main (void)
{
    test_easy_solve();

    return EXIT_SUCCESS;
}
