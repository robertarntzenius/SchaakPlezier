#pragma once

#include "humanplayer.h"
#include "randomplayer.h"
#include "minmaxplayer.h"
#include "montecarloplayer.h"

#include <memory>

class PlayerFactory {
public:
    PlayerFactory() = default;
    ~PlayerFactory() = default;
    [[nodiscard]] static std::unique_ptr<Player> makePlayer(const std::string &playerTypeString);
    [[nodiscard]] static std::unique_ptr<Player> makePlayer(const std::string &playerTypeString, PlayerSettings settings);
};
