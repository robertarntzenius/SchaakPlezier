#pragma once

#include "board.h"
#include "log.h"

#include <algorithm>

class Game {
    public:
        Game(const char *FENString = defaultStartingFEN);
        ~Game() = default;

        void start();
        void test();

    private:
        static bool parseMove(const std::vector<Move> &moves, std::string &input, Move &move);

        ChessLogger& logger;
        Board _board;
};