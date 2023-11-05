#include "game.h"
#include "log.h"

Game::Game() 
    : logger(ChessLogger::getInstance()), _board()
{
}

int *Game::getPossibleMoves()
{
    logger.log("Starting move generation for turn %d", _board.getTurn());
    return _board.getPossibleMoves();
}

bool Game::isOver()
{
    return true;
}

Color Game::switchTurn()
{
    return _board.switchTurn();
}

Color Game::getTurn()
{
    return _board.getTurn();
}
