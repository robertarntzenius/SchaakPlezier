#pragma once

#include "board.h"
#include "piece.h"


class Game {
    public:
        Game() = default;
        ~Game() = default;

        bool isOver();
        
    private:
        Board board;
        unsigned int turn;

};