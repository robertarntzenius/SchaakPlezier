#include <iostream>

#include "game.h"


int main() {
    Game* game = new Game(defaultStartingFEN);

    try {
//        game->start();
        game->test();

    } catch (std::string &e) {
        std::cerr << e << std::endl;
    }

    delete game;
}