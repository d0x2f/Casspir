#pragma once

#include <cstdint>
#include <vector>
#include <set>

#include "definitions.hh"

namespace Casspir
{
    class Map
    {
        public:
            Map(uint32_t width, uint32_t height, uint8_t difficulty, Point first_flip);
            Map(uint32_t width, uint32_t height, std::set<Casspir::Point> mines);

            uint64_t flip(Point position);
            void flag(Point position);
            void reset();

            uint32_t get_width();
            uint32_t get_height();
            std::vector<TileState>& get_state();
            uint64_t get_num_flipped();
            uint64_t get_mines_remaining();
            uint64_t get_total_mines();
            MapStatus get_status();

            TileState& get_tile(Point position);
            TileState& get_tile(uint64_t index);

            bool is_tile_satisfied(Point position);

            std::set<Point> get_neighbours(Point position);

            void print(bool revealed = false);

        private:
            Map(uint32_t width, uint32_t height);

            uint32_t width, height;
            uint64_t total_mines, mines_remaining, tiles_flipped;
            MapStatus status;
            std::vector<TileState> state;

            uint64_t flip_recurse(Point position);
            void check_completed();
    };
}
