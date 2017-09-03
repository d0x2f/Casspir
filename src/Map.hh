#pragma once

#include <cstdint>
#include <vector>

#include "definitions.hh"

namespace Casspir
{
    class Map
    {
        public:
            Map(uint32_t width, uint32_t height, uint8_t difficulty, Point click);

            void flip(Point position);

            uint32_t get_width();
            uint32_t get_height();
            std::vector<TileState> get_state();

        private:
            uint32_t width, height;
            uint8_t difficulty;
            std::vector<TileState> state;

            std::vector<Point> get_neighbours(Point position);
            void flip_recurse(Point position);
    };
}
