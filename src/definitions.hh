#pragma once

#include <cstdint>

namespace Casspir
{

    struct TileState {
        uint8_t value;
        bool mine;
        bool flagged;
        bool flipped;

        TileState(
            uint8_t value = 0,
            bool mine = false,
            bool flagged = false,
            bool flipped = false
        ) : value(value), mine(mine), flagged(flagged), flipped(flipped)
        {}
    };

    struct Point {
        uint32_t x, y;

        Point(uint32_t x, uint32_t y) : x(x), y(y) {};

        uint64_t get_index(uint32_t width) const
        {
            return y*width + x;
        }

        static Point from_index(uint64_t index, uint32_t width)
        {
            return Point(index % width, index / width);
        }

        bool operator==(const Point& other) const {
            return this->x == other.x && this->y == other.y;
        }

        bool operator!=(const Point& other) const {
            return !(*this == other);
        }
    };

    enum OperationType {
        FLIP,
        FLAG
    };

    struct Operation {
        OperationType type;
        Point position;

        Operation(
            OperationType type,
            Point position
        ) : type(type), position(position)
        {}
    };

    enum MapStatus {
        IN_PROGRESS,
        FAILED,
        COMPLETE
    };
}