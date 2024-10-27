#include "playerfactory.h"

PlayerFactory::PlayerFactory() {}


PlayerSettings readSettings(const std::string &filePath) {
    std::ifstream f(filePath);
    json data = json::parse(f);
    PlayerSettings settings;

    settings.MonteCarlo_Depth = data[playerTypeStringMap.at(MonteCarlo)]["depth"];
    settings.MonteCarlo_Breadth = data[playerTypeStringMap.at(MonteCarlo)]["breadth"];
    settings.AlphaBeta_Depth = data[playerTypeStringMap.at(AlphaBeta)]["depth"];
    settings.MinMax_Depth = data[playerTypeStringMap.at(MinMax)]["depth"];
    return settings;
};

std::unique_ptr<Player> PlayerFactory::makePlayer(const std::string &playerTypeString)
{
    // TODO think about this
    PlayerSettings settings = readSettings("../../settings.json");
    return makePlayer(playerTypeString, settings);
}

std::unique_ptr<Player> PlayerFactory::makePlayer(const std::string &playerTypeString, PlayerSettings settings) {
    if (!stringPlayerTypeMap.contains(playerTypeString)) {
        throw std::invalid_argument("Unknown player type string " + playerTypeString);
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
        std::cerr << playerType << std::endl;
        throw std::invalid_argument("Unknown player type");
    }
}
