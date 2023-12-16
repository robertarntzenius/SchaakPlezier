#include "game.h"
#include "log.h"

Game::Game() 
    : logger(ChessLogger::getInstance()), _board()
{
    
    #ifdef DEBUG
        logger.log("New game created!");
    #endif
}

void Game::start()
{
    // TODO implement

//    while (!this->isOver())
//    {
//          _assert(CheckBitboards())
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
    _board.addPiece(e5, wPawn, White);
    _board.addPiece(d5, bPawn, Black);
    _board.setEnPassant(d6);

    std::vector<Move> wmoves = _board.getPossibleMoves();

    logger.logHeader("wmoves in game.test()");
    _board.logBoard();

    for (const auto& move : wmoves)
        logger.log(move);
    _board.switchTurn();

    _board.addPiece(a4, wPawn, White);
    _board.addPiece(b4, bPawn, Black);
    _board.setEnPassant(a3);
    std::vector<Move> bmoves = _board.getPossibleMoves();

    logger.logHeader("bmoves in game.test()");
    _board.logBoard();

    for (const auto& move : bmoves)
        logger.log(move);
}

bool Game::isOver () const
{
    // TODO implement
    return false;
}