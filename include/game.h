#pragma once

#include "board.h"
#include "piece.h"
#include "log.h"


class Game {
    public:
        Game();
        ~Game() = default;
        int* getPossibleMoves();
        bool isOver();
        Color switchTurn();
        Color getTurn();
    private:
        ChessLogger& logger;
        Board _board;
};