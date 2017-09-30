#pragma once

#include <queue>
#include <memory>
#include <set>
#include <random>
#include <chrono>

#include "Map.hh"
#include "definitions.hh"

namespace Casspir
{
    class Solver
    {
        public:
            Solver(Map& map);
            std::queue<Operation> solve();

        protected:
            Map& map;
            uint64_t map_size;
            std::queue<Operation> operations;
            std::default_random_engine random_engine;
            std::uniform_int_distribution<uint64_t> random_int;

            bool perform_pass();
            bool evaluate_neighbours(uint64_t index);
            std::pair< std::set<Point>, std::set<Point> > find_group();
            bool enumerate_group(
                const std::set<Point> border_unflipped,
                const std::set<Point> border_flipped
            );
            void flip_random_tile();
            bool flip(Point position);
            bool flag(Point position);

            void recursive_border_search(
                Point position,
                std::set<Point>& border_unflipped,
                std::set<Point>& border_flipped
            );
    };
}
