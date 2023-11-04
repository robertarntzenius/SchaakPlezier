#include <iostream>
#include "game.h"


int main() {
    Game* game = new Game();

    
    // for testing purposes, play 2 turns
    for (int i = 0; i < 2; i++) {
        game->getPossibleMoves();
        game->switchTurn();
    }
    //    while(!game->isOver() ) {
    //
    //    }
    delete game;
}