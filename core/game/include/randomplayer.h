#pragma once

#include "player.h"
#include <ctime>

class RandomPlayer : public Player {
    public:
        RandomPlayer() { std::srand(static_cast<unsigned int>(std::time(nullptr))); }
        int decideOnMove (Board boardCopy, const std::vector<Move> &moves) override { return (std::rand() % moves.size()); }
        PlayerType getPlayerType() override {return Random;};
    private:
};