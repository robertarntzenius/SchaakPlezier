#include "game.h"

Game::Game() 
    : board()
{
}

int *Game::getPossibleMoves()
{
    return board.getPossibleMoves();
}

bool Game::isOver()
{
    return true;
}

void Game::switchTurn()
{
    board.switchTurn();
}
