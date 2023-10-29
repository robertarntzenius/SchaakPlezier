#pragma once

#include "board.h"
#include "piece.h"


class Game {
    public:
        Game();
        ~Game() = default;

        bool isOver();
        
    private:
        Board board;
};