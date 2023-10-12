#pragma once

#include "include/board.h"
#include "include/piece.h"



class Game {
    public:
        Game() = default;
        ~Game() = default;
        int isOver();
        
    private:
        Board board;
        unsigned int turn;

};