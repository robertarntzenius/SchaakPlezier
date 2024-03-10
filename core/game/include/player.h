#pragma once

#include "board.h"
#include "evaluation.h"


enum PlayerType {
    Human,
    Random,
    MinMax,
    MonteCarlo,
};

class Player {
    public:
        Player() = default;
        virtual ~Player() = default;

        // TODO add timestamp
        [[nodiscard]] virtual size_t decideOnMove (Board boardCopy, const std::vector<Move> &moves) = 0;
        [[nodiscard]] virtual PlayerType getPlayerType() = 0;
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
};

const std::unordered_map<PlayerType, std::string> playerTypeStringMap = {
    {Human, "Human"},
    {Random, "Random"},
    {MinMax, "MinMax"},
    {MonteCarlo, "MonteCarlo"},
        
};


static std::ostream& operator<<(std::ostream &os, const PlayerType &playerType) {
    os << playerTypeStringMap.at(playerType);
    return os;
}
