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
        logger.log(move);

    _board.doMove(moves[0]);
    _board.switchTurn();
    
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
