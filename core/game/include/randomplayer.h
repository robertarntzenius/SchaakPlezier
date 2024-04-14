#pragma once

#include "player.h"
#include <random>

class RandomPlayer : public Player {
public:
    RandomPlayer(std::mt19937::result_type seed = std::random_device{}())
    : gen(seed)
    {
    }
    
    RandomPlayer(PlayerSettings settings)
    : gen(settings.Random_Seed)
    {
    }

    [[nodiscard]] size_t decideOnMove(Board boardCopy, const std::vector<Move> &moves) override {
        std::uniform_int_distribution<> dist(0, moves.size() - 1);
        return dist(gen);
    }

    [[nodiscard]] PlayerType getPlayerType() override { return Random; };
    [[nodiscard]] json getSettings() override {
        json settings = {
            {"playerType", playerTypeStringMap.at(Random)}
        };
        return settings;
    };


    void marie(Board boardCopy, const std::vector<Move> &moves) {
        decideOnMove(boardCopy, moves);
    }

private:
    std::mt19937 gen;
};