#pragma once

#include <vector>
#include <cstdint>

namespace Casspir
{
    enum TileValues {
        ZERO = 0,
        ONE = 1,
        TWO = 2,
        THREE = 3,
        FOUR = 4,
        FIVE = 5,
        SIX = 6,
        SEVEN = 7,
        EIGHT = 8,
        BOMB = 9
    };
}

std::vector<Casspir::TileValues> generate_map(uint32_t n, uint32_t m, uint8_t difficulty);