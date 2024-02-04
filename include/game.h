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
        static bool parsePromotionMove(const std::vector<Move> &moves, std::iterator<const Move *, std::vector<Move>> it, std::string &userInput, Move &move);

        ChessLogger& logger;
        Board _board;
};