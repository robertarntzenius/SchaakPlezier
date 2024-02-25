#pragma once

#include "player.h"

class HumanPlayer : public Player {
    public:
        [[nodiscard]] size_t decideOnMove (Board boardCopy, const std::vector<Move> &moves, const BoardState &copyState) override;
        [[nodiscard]] PlayerType getPlayerType() override { return Human; };

    private:
        [[nodiscard]] static bool parseMove(const std::vector<Move> &moves, std::string& userInput, size_t &moveIndex);
};
