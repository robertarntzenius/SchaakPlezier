#include "playerfactory.h"

std::unique_ptr<Player> PlayerFactory::makePlayer(const std::string &playerTypeString) {
    if (!stringPlayerTypeMap.contains(playerTypeString)) {
        throw std::invalid_argument("Unknown player type");
    }
        
    PlayerType playerType = stringPlayerTypeMap.at(playerTypeString);
    switch (playerType)
    {
    case Human:
        return std::make_unique<HumanPlayer>();
    case Random:
        return std::make_unique<RandomPlayer>();
   case MinMax:
       return std::make_unique<MinMaxPlayer>();
    default:
        throw std::invalid_argument("Unknown player type");
    }
}
