#pragma once

#include "board.h"
#include "log.h"

#include "randomplayer.h"

#include <algorithm>

class Game {
    public:
        explicit Game(const char *FENString = defaultStartingFEN);
        ~Game() = default;

        void start();

    private:
        static bool parseMove(const std::vector<Move> &moves, std::string &input, Move &move);

        ChessLogger& logger;
        Board board;
};