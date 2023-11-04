#pragma once

#include "board.h"
#include "piece.h"


class Game {
    public:
        Game();
        ~Game() = default;
        int* getPossibleMoves();
        bool isOver();
        void switchTurn();
    private:
        Board board;
};