#include <cassert>
#include <cstdlib>
#include <vector>

#include <casspir.hh>
#include <Solver.hh>

static void test_easy_solve()
{
    std::vector<Casspir::Point> mines = {
        Casspir::Point(5,1),
        Casspir::Point(6,3),
        Casspir::Point(1,4),
        Casspir::Point(8,7),
        Casspir::Point(8,9),
        Casspir::Point(2,4),
        Casspir::Point(6,4),
        Casspir::Point(8,3),
        Casspir::Point(2,8),
        Casspir::Point(0,1)
    };
    Casspir::Map map = casspir_make_map(10,10, mines);

    //The number of tiles flipped should be 0.
    assert( map.get_num_flipped() == 0 );

    map.flip(Casspir::Point(6,9));

    Casspir::Solver solver(map);
    solver.solve();

    assert( map.get_status() == Casspir::MapStatus::COMPLETE );
}

int main (void)
{
    test_easy_solve();

    return EXIT_SUCCESS;
}
