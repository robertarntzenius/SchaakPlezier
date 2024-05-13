#pragma once

#include "player.h"

class HumanPlayer : public Player {
public:
    HumanPlayer(PlayerSettings settings) {};
    [[nodiscard]] Move decideOnMove (Board boardCopy, const std::vector<Move> &moves) override;
    [[nodiscard]] PlayerType getPlayerType() override { return Human; };
    
    [[nodiscard]] json getSettings() override;
private:
    [[nodiscard]] static bool parseMove(const std::vector<Move> &moves, std::string& userInput, size_t &moveIndex);
};
