#pragma once

#include "board.h"

#define MAX_EVAL 100.0
#define MIN_EVAL (-100.0)


constexpr std::array<double, NrPiecetypes> pieceValues {
    1.0,
    3.0,
    3.25,
    5.0,
    9.0,
    0
};
