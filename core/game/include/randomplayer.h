#pragma once

#include "player.h"
#include <random>

class RandomPlayer : public Player {
    public:
        RandomPlayer()
        : randomDevice(),
          gen(randomDevice())
        {
        }

        [[nodiscard]] size_t decideOnMove(Board boardCopy, const std::vector<Move> &moves, const BoardState &copyState) override {
            std::uniform_int_distribution<> dist(0, moves.size());
            return dist(gen);
        }

        [[nodiscard]] PlayerType getPlayerType() override { return Random; };

    private:
        std::random_device randomDevice;
        std::mt19937 gen;
};