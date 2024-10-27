#pragma once

#include "board.h"
#include "evaluation.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;


enum PlayerType {
    Human,
    Random,
    MinMax,
    MonteCarlo,
    AlphaBeta
};

// TODO refactor? maybe config file?
struct PlayerSettings {
    int MonteCarlo_Depth = 10;
    int MonteCarlo_Breadth = 10;

    int MinMax_Depth = 4;
    int AlphaBeta_Depth = 6;


    int MonteCarlo_Seed = 12;
    int Random_Seed = 6;
};


class Player {
    public:
        Player() = default;
        virtual ~Player() = default;

        // TODO add timestamp
        [[nodiscard]] virtual Move decideOnMove (Board boardCopy, const std::vector<Move> &moves) = 0;
        [[nodiscard]] virtual PlayerType getPlayerType() = 0;
        [[nodiscard]] virtual json getSettings() = 0;
};

const std::unordered_map<std::string, PlayerType> stringPlayerTypeMap = {
    {"human", Human},
    {"Human", Human},
    {"random", Random},
    {"Random", Random},
    {"MinMax", MinMax},
    {"minmax", MinMax},
    {"MonteCarlo", MonteCarlo},
    {"montecarlo", MonteCarlo},
    {"AlphaBeta", AlphaBeta},
    {"alphabeta", AlphaBeta}
};

const std::unordered_map<PlayerType, std::string> playerTypeStringMap = {
    {Human, "Human"},
    {Random, "Random"},
    {MinMax, "MinMax"},
    {MonteCarlo, "MonteCarlo"},
    {AlphaBeta, "AlphaBeta"},
};


static std::ostream& operator<<(std::ostream &os, const PlayerType &playerType) {
    os << playerTypeStringMap.at(playerType);
    return os;
}
