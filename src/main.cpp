#include <iostream>
#include "game.h"


int main() {
    Game* game = new Game();
    
    while(!game->isOver()) {
        
    }
    delete game;
}