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

            void perform_pass();
            void evaluate_neighbours(uint64_t index);
            void flip(Point position);
            void flag(Point position);
    };
}
