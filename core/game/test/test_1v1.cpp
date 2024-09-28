#include "game.h"
#include "playerfactory.h"

#include <iostream>
#include <unordered_map>
#include <memory>
#include <cstring>
#include <dlfcn.h>  // For dynamic loading (Unix)
#include <unistd.h>

// Function prototype for loading PlayerFactory from game.so
typedef PlayerFactory* (*GetPlayerFactoryFunc)();


int runChessBattle(PlayerFactory* factory1, PlayerFactory* factory2, std::string white, std::string black, const char *selectedFEN, const int nrGames) {
    // Determine white player (from the first game.so)
    std::unique_ptr<Player> whitePlayer = factory1->makePlayer(white);
    if (!whitePlayer) {
        std::cerr << "Failed to create white player: " << white << " from game library 1" << std::endl;
        return 1;
    }

    // Determine black player (from the second game.so)
    std::unique_ptr<Player> blackPlayer = factory2->makePlayer(black);
    if (!blackPlayer) {
        std::cerr << "Failed to create black player: " << black << " from game library 2" << std::endl;
        return 1;
    }

    // Initialize the game with the selected players
    Game game(std::move(whitePlayer), std::move(blackPlayer), selectedFEN);

    // Track game results
    std::unordered_map<GameResult, size_t> gameresults = {
        {NOT_OVER, 0},
        {WHITE_WIN_BY_CHECKMATE, 0},
        {BLACK_WIN_BY_CHECKMATE, 0},
        {WHITE_WIN_BY_TIME_OUT, 0},
        {BLACK_WIN_BY_TIME_OUT, 0},
        {WHITE_WIN_BY_FORFEIT, 0},
        {BLACK_WIN_BY_FORFEIT, 0},
        {DRAW_BY_STALEMATE, 0},
        {DRAW_BY_INSUFFICIENT_MATERIAL, 0},
        {DRAW_BY_REPETITION, 0},
        {DRAW_BY_50_MOVES, 0},
    };

    try {
        for (int i = 0; i < nrGames; ++i) {
            GameResult result = game.start(false);
            std::cout << result << " (" << i + 1 << "/" << nrGames << ")\n";

            gameresults[result]++;
            game.resetBoard();
        }
    } catch (const std::exception &e) {
        std::cerr << "Error during game: " << e.what() << std::endl;
        return 1;
    }

    // Output the results
    auto print_result_count = [](const auto& result, const auto& count) {
        std::cout << result << ": " << count << "\n";
    };

    for (const auto &pair : gameresults) {
        print_result_count(pair.first, pair.second);
    }

    return 0;
}


int main(int argc, char* argv[]) {
    // Check for correct number of arguments
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <game1.so> <game2.so> <white_player> <black_player> [nr_games]" << std::endl;
        return 1;
    }

    const char* game1_lib = argv[1];
    const char* game2_lib = argv[2];
    const char* white = argv[3];
    const char* black = argv[4];
    const int nrGames = (argc >= 6) ? std::stoi(argv[5]) : 25;  // Default to 25 games if not specified
    const char* selectedFEN = defaultStartingFEN;  // Using default FEN for chess start position

    // Load the game.so files
    void* handle1 = dlopen(game1_lib, RTLD_LAZY);
    if (!handle1) {
        std::cerr << "Failed to open game library 1: " << dlerror() << std::endl;
        return 1;
    }
    void* handle2 = dlopen(game2_lib, RTLD_LAZY);
    if (!handle2) {
        std::cerr << "Failed to open game library 2: " << dlerror() << std::endl;
        dlclose(handle1);
        return 1;
    }

    // Clear any existing errors
    dlerror();

    // Load the PlayerFactory from the first version of game.so
    GetPlayerFactoryFunc getPlayerFactory1 = (GetPlayerFactoryFunc) dlsym(handle1, "getPlayerFactory");
    const char* dlsymError1 = dlerror();
    if (dlsymError1) {
        std::cerr << "Cannot load symbol 'getPlayerFactory' from game library 1: " << dlsymError1 << std::endl;
        dlclose(handle1);
        dlclose(handle2);
        return 1;
    }

    // Load the PlayerFactory from the second version of game.so
    GetPlayerFactoryFunc getPlayerFactory2 = (GetPlayerFactoryFunc) dlsym(handle2, "getPlayerFactory");
    const char* dlsymError2 = dlerror();
    if (dlsymError2) {
        std::cerr << "Cannot load symbol 'getPlayerFactory' from game library 2: " << dlsymError2 << std::endl;
        dlclose(handle1);
        dlclose(handle2);
        return 1;
    }

    // Get the PlayerFactory instances
    PlayerFactory* playerFactory1 = getPlayerFactory1();
    PlayerFactory* playerFactory2 = getPlayerFactory2();

    // Play white vs black
    if (runChessBattle(playerFactory1, playerFactory2, white, black, selectedFEN, nrGames)) {
        dlclose(handle1);
        dlclose(handle2);
        return 1;
    }

    // Play black vs white
    if (runChessBattle(playerFactory1, playerFactory2, black, white, selectedFEN, nrGames)) {
        dlclose(handle1);
        dlclose(handle2);
        return 1;
    }

    // Close the libraries
    dlclose(handle1);
    dlclose(handle2);

    return 0;
}
