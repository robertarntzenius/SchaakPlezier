#pragma once

#include "player.h"

class RandomPlayer : public Player {
    public:
//        RandomPlayer ();

        int decideOnMove (Board boardCopy, const std::vector<Move> &moves) override { return rand() % moves.size(); }

    private:
//        int search (const Board &board, const std::vector<Move> &moves);
//        double evaluate (const Board &board);
};