#pragma once

#include "board.h"
#include "log.h"


class Game {
    public:
        Game(const char *FENString = defaultStartingFEN);
        ~Game() = default;

        void start();
        void test();
        void logMove(const std::unique_ptr<Move> &move) const;

    private:
        bool isOver() const;

        ChessLogger& logger;
        Board _board;
};