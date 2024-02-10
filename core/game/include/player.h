#pragma once

#include "board.h"
#include <memory>

class Player {
    public:
        Player() = default;
        virtual ~Player() = default;

        // TODO add timestamp
        virtual int decideOnMove (Board boardCopy, const std::vector<Move> &moves) = 0;
        virtual PlayerType getPlayerType() = 0;
};
