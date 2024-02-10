#include "playerfactory.h"

std::unique_ptr<Player> PlayerFactory::makePlayer(const std::string &playerTypeString) {
    if (!stringPlayerTypeMap.contains(playerTypeString)) {
        throw std::invalid_argument("Unknown player type");
        return nullptr;
    }
        
    PlayerType playerType = stringPlayerTypeMap.at(playerTypeString);
    switch (playerType)
    {
    case Human:
        return std::make_unique<HumanPlayer>();
        break;
    case Random:
        return std::make_unique<RandomPlayer>();
        break;
    default:
        throw std::invalid_argument("Unknown player type");
        return nullptr;
        break;
    }
}
