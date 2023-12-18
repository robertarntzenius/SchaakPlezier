#pragma once

#include "board.h"
#include "log.h"


class Game {
    public:
        Game(const std::string& FENString = defaultStartingFEN);
        ~Game() = default;

        void start();
        void test();
    private:
        bool isOver() const;

        ChessLogger& logger;
        Board _board;
};