#pragma once

#include <queue>
#include <memory>

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
            std::queue<Operation> operations;

            bool perform_pass();
            bool evaluate_neighbours(uint64_t index);
            bool flip(Point position);
            bool flag(Point position);
    };
}
