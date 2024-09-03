#include "game.h"
#include "playerfactory.h"

#include <iostream>
#include <unistd.h>


int main(int argc, char *argv[]) {

    const char *selectedFEN = defaultStartingFEN;
    PlayerFactory playerFactory;
    std::unique_ptr<Player> whitePlayer = PlayerFactory::makePlayer("Human");
    std::unique_ptr<Player> blackPlayer = PlayerFactory::makePlayer("Human");

    int opt = 0;
    while ((opt = getopt(argc, argv, "f:w:b:")) != -1) {
        switch (opt) {
            case 'f':
                // Set the selected FEN from the option argument
                selectedFEN = optarg;
                break;
            case 'w':
                whitePlayer = PlayerFactory::makePlayer(optarg);
                break;
            case 'b':
                blackPlayer = PlayerFactory::makePlayer(optarg);
                break;
            case '?':
                // Handle unknown or missing option argument
                break;
            default:
                // Handle other options
                break;
        }
    }

    Game game(std::move(whitePlayer), std::move(blackPlayer), selectedFEN);

    try {
        game.start();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
