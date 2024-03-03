#pragma once

#include "player.h"
#include <ctime>

class RandomPlayer : public Player {
    public:
        RandomPlayer() { std::srand(static_cast<unsigned int>(std::time(nullptr))); }
        [[nodiscard]] size_t decideOnMove(Board boardCopy, const std::vector<Move> &moves, const BoardState &copyState) override { 
            return (std::rand() % moves.size());
        }
        [[nodiscard]] PlayerType getPlayerType() override { return Random; };
};