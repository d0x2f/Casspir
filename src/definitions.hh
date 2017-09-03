#pragma once

#include <cstdint>

namespace Casspir
{
    enum TileValue {
        ZERO = 0,
        ONE = 1,
        TWO = 2,
        THREE = 3,
        FOUR = 4,
        FIVE = 5,
        SIX = 6,
        SEVEN = 7,
        EIGHT = 8,
        MINE = 9
    };

    struct TileState {
        TileValue value;
        bool flipped;

        TileState(TileValue value = ZERO, bool flipped = false) : value(value), flipped(flipped) {}
    };

    struct Point {
        uint32_t x, y;

        Point(uint32_t x, uint32_t y) : x(x), y(y) {};

        uint64_t get_index()
        {
            return x*y;
        }
    };
}