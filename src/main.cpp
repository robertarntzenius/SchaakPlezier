#include <iostream>

#include "game.h"
#include "log.h" // static logger is created here so all files can use an instance of it


int main() {
    
    ChessLogger& logger = ChessLogger::getInstance();

    Game* game = new Game();
    logger.log("New game created!");
    
    // for testing purposes, play 5 turns
    for (int i = 0; i < 5; i++) {
        game->getPossibleMoves();
        game->switchTurn();
    }

    delete game;
}