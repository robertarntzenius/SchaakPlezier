#pragma once

#include "player.h"

class HumanPlayer : public Player {
    public:
        [[nodiscard]] int decideOnMove (Board boardCopy, const std::vector<Move> &moves) override;
        [[nodiscard]] PlayerType getPlayerType() override { return Human; };

    private:
        [[nodiscard]] static bool parseMove(const std::vector<Move> &moves, std::string& userInput, int &moveIndex);
};
