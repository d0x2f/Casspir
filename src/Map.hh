#pragma once

#include <cstdint>
#include <vector>

#include "definitions.hh"

namespace Casspir
{
    class Map
    {
        public:
            Map(uint32_t width, uint32_t height, uint8_t difficulty, Point first_flip);
            Map(uint32_t width, uint32_t height, std::vector<Casspir::Point> mines);

            void flip(Point position);
            void flag(Point position);

            uint32_t get_width();
            uint32_t get_height();
            std::vector<TileState> get_state();
            uint64_t get_num_flipped();
            uint64_t get_mines_remaining();
            MapStatus get_status();
            TileState& get_tile(Point position);

            void print();

        private:
            Map(uint32_t width, uint32_t height);

            uint32_t width, height;
            uint64_t mines_remaining, tiles_flipped;
            MapStatus status;
            std::vector<TileState> state;

            std::vector<Point> get_neighbours(Point position);
            void flip_recurse(Point position);
    };
}
