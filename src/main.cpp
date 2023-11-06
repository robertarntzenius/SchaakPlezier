#include <iostream>

#include "game.h"


int main() {
    Game* game = new Game();

    try {
//        game->start();
        game->test();

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    delete game;
}