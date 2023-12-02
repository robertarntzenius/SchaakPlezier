#include "game.h"
#include "log.h"

Game::Game() 
    : logger(ChessLogger::getInstance()), _board()
{
    logger.log("New game created!");
}

void Game::start()
{
    // TODO implement

//    while (!this->isOver())
//    {
//        Move *possibleMoves = _board.getPossibleMoves();
//
//        // get active player to decide on one of the possible moves
//
//        selectedMove = ()? : ;
//
//
//        _board.doMove(selectedMove);
//
//        _board.switchTurn();
//    }
}

void Game::test()
{
    std::vector<Move> moves = _board.getPossibleMoves();
    for (const auto& move : moves)
        logger.log("Moves: %d %d %d %d", move.from, move.to, move.pType, move.capture);

    _board.switchTurn();

    moves = _board.getPossibleMoves();
}

bool Game::isOver () const
{
    // TODO implement
    return false;
}



//int *Game::getPossibleMoves()
//{
//    logger.log("Starting move generation for player %d", _board.getTurn());
//    return _board.getPossibleMoves();
//}
//
//Color Game::switchTurn()
//{
//    return _board.switchTurn();
//}
//
//Color Game::getTurn()
//{
//    return _board.getTurn();
//}
