#include "playerfactory.h"
std::unique_ptr<Player> PlayerFactory::makePlayer(const std::string &playerTypeString) {
    PlayerSettings settings;
    return makePlayer(playerTypeString, settings);
}

std::unique_ptr<Player> PlayerFactory::makePlayer(const std::string &playerTypeString, PlayerSettings settings) {
    if (!stringPlayerTypeMap.contains(playerTypeString)) {
        throw std::invalid_argument("Unknown player type");
    }

    PlayerType playerType = stringPlayerTypeMap.at(playerTypeString);
    switch (playerType)
    {
    case Human:
        return std::make_unique<HumanPlayer>(settings);
    case Random:
        return std::make_unique<RandomPlayer>(settings);
    case MinMax:
       return std::make_unique<MinMaxPlayer>(settings);
    case AlphaBeta:
        return std::make_unique<AlphaBetaPlayer>(settings);
    case MonteCarlo:
        return std::make_unique<MonteCarloPlayer>(settings);
    default:
        throw std::invalid_argument("Unknown player type");
    }
}
