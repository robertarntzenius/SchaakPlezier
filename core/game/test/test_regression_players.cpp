#include "game.h"
#include "playerfactory.h"

#include <filesystem>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <nlohmann/json.hpp>

void createFolder(const std::string& folderPath) {
    if (!std::filesystem::exists(folderPath)) {
        std::filesystem::create_directory(folderPath);
    }
}

void saveJSONToFile(const std::string& filePath, const json& data) {
    std::ofstream file(filePath);

    if (file.is_open()) {
        file << data.dump(4) << std::endl;
        file.close();
    } else {
        std::cerr << "Failed to save JSON data: " << data << "\nto file: " << filePath << std::endl;
    }
}

json loadJSON(const std::string& filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
    }
    json jsonData;
    try {
        file >> jsonData;
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
        file.close();
    }
    file.close();
    return jsonData;
}

void testRegressionPlayers(std::string &testPlayer, PlayerSettings settings, std::string testVar, int nrGames = 100, const char *selectedFEN = defaultStartingFEN) {
    PlayerFactory factory;
    std::unique_ptr<Player> whitePlayer = factory.makePlayer(testPlayer, settings);
    std::unique_ptr<Player> blackPlayer = factory.makePlayer("Random", settings);

    Game game(std::move(whitePlayer), std::move(blackPlayer), selectedFEN);

    std::unordered_map<GameResult, size_t> gameresults;
    auto start = std::chrono::steady_clock::now();

    try {
        for (int i = 0; i < nrGames; i++) {
            GameResult result = game.start(false);
            gameresults[result]++;
            std::cout << i + 1 << "/" << nrGames << ": " << result << std::endl;
            game.resetBoard();
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    auto end = std::chrono::steady_clock::now();
    auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000;

    // Calculate and print metrics
    size_t whiteWins = gameresults[WHITE_WIN_BY_CHECKMATE] + gameresults[WHITE_WIN_BY_TIME_OUT] + gameresults[WHITE_WIN_BY_FORFEIT];
    size_t blackWins = gameresults[BLACK_WIN_BY_CHECKMATE] + gameresults[BLACK_WIN_BY_TIME_OUT] + gameresults[BLACK_WIN_BY_FORFEIT];
    size_t draws = gameresults[DRAW_BY_STALEMATE] + gameresults[DRAW_BY_INSUFFICIENT_MATERIAL] + gameresults[DRAW_BY_REPETITION] + gameresults[DRAW_BY_50_MOVES];

    // Round these to 2 decimals
    double whiteWinRate = (double)whiteWins / nrGames * 100;
    double blackWinRate = (double)blackWins / nrGames * 100;
    double drawRate = (double)draws / nrGames * 100;

    // Metrics to JSON object
    json currentResult;
    currentResult["whitePlayer"] = game.getPlayerSettings(White);
    currentResult["blackPlayer"] = game.getPlayerSettings(Black);
    currentResult["metrics"]["duration (s)"] = duration_seconds;
    currentResult["metrics"]["winRate (%)"] = whiteWinRate;
    currentResult["metrics"]["drawRate (%)"] = drawRate;
    currentResult["metrics"]["nrGames (#)"] = nrGames;

    for (const auto &pair : gameresults) {
        std::string resultString = gameResultStringMap.at(pair.first);
        currentResult["gameresults"][resultString] = static_cast<int>(pair.second);
    }

    std::string folderPath = "core/game/test/regression/white/" + currentResult["whitePlayer"]["playerType"].get<std::string>();
    std::string filePath = folderPath + "/" + testVar + std::to_string(game.getPlayerSettings(White)[testVar].get<int>()) + ".json";

    // Assert not worse
    // Tests will fail if winrate goes down by tolerance %
    double tolerance = 5;

    if (std::filesystem::exists(filePath)) {
        json previousResult = loadJSON(filePath);
        double previousWinRate = previousResult["metrics"]["winRate (%)"].get<double>();
        if (!(whiteWinRate >= previousWinRate - tolerance)) {
            std::cerr << "whiteWinRate: " << whiteWinRate << ", previousWinRate: " << previousWinRate << ", tolerance: " << tolerance << std::endl;
        }
        assert(whiteWinRate >= previousWinRate - tolerance);
    }

    createFolder(folderPath);
    saveJSONToFile(filePath, currentResult);
}

void testRegressionMinMax(int maxDepth) {
    PlayerFactory factory;
    PlayerSettings settings;
    std::string testPlayer = "MinMax";
    std::string testVar = "maxDepth";

    for (int currentDepth=1; currentDepth < maxDepth; currentDepth++) {
        settings.MinMax_Depth = currentDepth;
        testRegressionPlayers(testPlayer, settings, testVar);
    }
}

void testRegressionMonteCarloDepth(int maxDepth) {
    PlayerFactory factory;
    PlayerSettings settings;
    std::string testPlayer = "MonteCarlo";
    std::string testVar = "movesPerGame";

    for (int depth = 10; depth < maxDepth; depth += 10) {
        settings.MonteCarlo_Depth = depth;
        testRegressionPlayers(testPlayer, settings, testVar);
    }
}

void testRegressionMonteCarloBreadth(int maxBreadth) {
    PlayerFactory factory;
    PlayerSettings settings;
    std::string testPlayer = "MonteCarlo";
    std::string testVar = "nrGames";

    for (int breadth = 10; breadth < maxBreadth; breadth += 10) {
        settings.MonteCarlo_Breadth = breadth;
        testRegressionPlayers(testPlayer, settings, testVar);
    }
}

int main() {
    testRegressionMinMax(5);

    // testRegressionMonteCarloDepth(100);
    // testRegressionMonteCarloBreadth(100);

    return 0;
}
