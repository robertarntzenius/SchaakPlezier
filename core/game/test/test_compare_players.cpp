#include "game.h"
#include "playerfactory.h"

#include <iostream>
#include <cstring>
#include <unistd.h>


int main(int argc, char *argv[]) {
    const int nrGames = 100;

    const char *selectedFEN = defaultStartingFEN;
    PlayerFactory playerFactory;
    std::unique_ptr<Player> whitePlayer = PlayerFactory::makePlayer("Random");
    std::unique_ptr<Player> blackPlayer = PlayerFactory::makePlayer("MinMax");

    Game game(std::move(whitePlayer), std::move(blackPlayer), selectedFEN);
    std::unordered_map<GameResult, size_t> gameresults;

    try {
        for (int i = 0; i < nrGames; i++) {
            GameResult result = game.start(false);
            gameresults[result]++;
            
	    std::cout << "Game nr: " << i << " (out of " << nrGames << ")\n"; 

            game.resetBoard();
        }

        // Helper lambda function to print key-value pairs
        auto print_result_count = [](const auto& result, const auto& count) {
            std::cout << result << ": " << count << "\n";
        };

        for (const auto &pair : gameresults) {
            print_result_count(pair.first, pair.second);
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
