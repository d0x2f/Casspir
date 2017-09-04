#pragma once

#include <vector>
#include <cstdint>

#include "definitions.hh"
#include "Map.hh"

Casspir::Map casspir_generate_map(
    uint32_t w,
    uint32_t h,
    uint8_t difficulty,
    Casspir::Point click
);

Casspir::Map casspir_make_map(
    uint32_t w,
    uint32_t h,
    std::vector<Casspir::Point> mines
);

Casspir::Map casspir_evaluate_flip(Casspir::Map map, Casspir::Point position);

std::vector<Casspir::Point> casspir_solve(Casspir::Map map);