#pragma once

#include "board.h"


enum PlayerType {
    Human,
    Random,
    MinMax,
};

class Player {
    public:
        Player() = default;
        virtual ~Player() = default;

        // TODO add timestamp
        [[nodiscard]] virtual size_t decideOnMove (Board boardCopy, const std::vector<Move> &moves, const BoardState &copyState) = 0;
        [[nodiscard]] virtual PlayerType getPlayerType() = 0;
};

const std::unordered_map<std::string, PlayerType> stringPlayerTypeMap = {
    {"human", Human},
    {"Human", Human},
    {"random", Random},
    {"Random", Random}, 
    {"MinMax", MinMax}, 
    {"minmax", MinMax}, 
};

const std::unordered_map<PlayerType, std::string> playerTypeStringMap = {
    {Human, "Human"},
    {Random, "Random"},
    {MinMax, "MinMax"},
};


static std::ostream& operator<<(std::ostream &os, const PlayerType &playerType) {
    os << playerTypeStringMap.at(playerType);
    return os;
}