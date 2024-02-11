#pragma once

#include "board.h"


enum PlayerType {
    Human,
    Random,
};

class Player {
    public:
        Player() = default;
        virtual ~Player() = default;

        // TODO add timestamp
        virtual int decideOnMove (Board boardCopy, const std::vector<Move> &moves) = 0;
        virtual PlayerType getPlayerType() = 0;
};

const std::unordered_map<std::string, PlayerType> stringPlayerTypeMap = {
    {"human", Human},
    {"Human", Human},
    {"random", Random},
    {"Random", Random},
};

const std::unordered_map<PlayerType, std::string> playerTypeStringMap = {
    {Human, "Human"},
    {Random, "Random"},
};


static std::ostream& operator<<(std::ostream &os, const PlayerType &playerType) {
    os << playerTypeStringMap.at(playerType);
    return os;
}