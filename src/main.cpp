#include <iostream>

#include "game.h"


int main() {
    Game* game = new Game(testFEN2);

    try {
//        game->start();
        game->test();

    } catch (std::string &e) {
        std::cerr << e << std::endl;
    }

    delete game;
}