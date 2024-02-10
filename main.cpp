#include "game.h"
#include "playerfactory.h"

#include <iostream>
#include <cstring>
#include <unistd.h>


int main(int argc, char *argv[]) {
    
    const char *selectedFEN = defaultStartingFEN;
    PlayerFactory playerFactory;
    std::unique_ptr<Player> player1 = playerFactory.makePlayer("Human");
    std::unique_ptr<Player> player2 = playerFactory.makePlayer("Human");

    int opt;
    while ((opt = getopt(argc, argv, "f:w:b:")) != -1) {
        switch (opt) {
            case 'f':
                // Set the selected FEN from the option argument
                selectedFEN = optarg;
                break;
            case 'w':
                player1 = playerFactory.makePlayer(optarg);
                break;
            case 'b':
                player2 = playerFactory.makePlayer(optarg);
                break;
            case '?':
                // Handle unknown or missing option argument
                break;
            default:
                // Handle other options
                break;
        }
    }

    Game game(std::move(player1), std::move(player2), selectedFEN);

    try {
        game.start();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}