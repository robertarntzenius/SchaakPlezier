#pragma once

#include "humanplayer.h"
#include "randomplayer.h"
#include <memory>

class PlayerFactory {
public:
    PlayerFactory() = default;
    ~PlayerFactory() = default;

    std::unique_ptr<Player> makePlayer(const std::string &playerTypeString);
};