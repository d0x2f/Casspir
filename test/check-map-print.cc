#include <cassert>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <set>

#include <casspir.hh>

static void test_map_print()
{
    std::set<Casspir::Point> mines = {
        Casspir::Point(1,3),
        Casspir::Point(3,2),
        Casspir::Point(1,1),
        Casspir::Point(6,3),
        Casspir::Point(7,2),
        Casspir::Point(9,3),
        Casspir::Point(0,5),
        Casspir::Point(2,2),
        Casspir::Point(8,6),
        Casspir::Point(3,3)
    };
    Casspir::Map map = casspir_make_map(10,10, mines);

    //The number of tiles flipped should be 0.
    assert( map.get_num_flipped() == 0 );

    map.flag(Casspir::Point(1,9));
    map.flip(Casspir::Point(5,5));
    map.flip(Casspir::Point(5,1));
    map.flag(Casspir::Point(7,3));
    map.flip(Casspir::Point(7,2));

    //The number of tiles flipped should be 72.
    assert( map.get_num_flipped() == 73 );

    //The game should be in progress
    assert( map.get_status() == Casspir::MapStatus::FAILED );


    //Redirect cout to string stream
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::ostringstream cout_capture;
    std::cout.rdbuf(cout_capture.rdbuf());

    map.print();

    //Restore cout
    std::cout.rdbuf(coutbuf);

    std::string map_string =
        "\n##10000000\n##32101110\n####212*21\n#######^##\n#2211111##\n#1000001##\n11000001##\n0000000111\n0000000000\n0^00000000\n";

    assert(cout_capture.str() == map_string);
}

int main (void)
{
    test_map_print();

    return EXIT_SUCCESS;
}
