#pragma once

#include "player.h"

class HumanPlayer : public Player {
    public:
        int decideOnMove (Board boardCopy, const std::vector<Move> &moves) override;

    private:
        bool parseMove(const std::vector<Move> &moves, std::string& userInput, Move &move);
};
