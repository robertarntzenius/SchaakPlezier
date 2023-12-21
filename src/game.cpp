#include "game.h"
#include "log.h"

Game::Game(const std::string& FENString) 
    : logger(ChessLogger::getInstance()), _board(Board(FENString))
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
    for (int i = 0; i < 20; i++) {

        std::vector<Move> moves = _board.getPossibleMoves();

        logger.logHeader("moves in game.test()");
        logger.log(i);

        for (Move move : moves)
        {
            logger.log(move);
            bool moveIsLegal = _board.makeMove(move);
            if (moveIsLegal) {
                break;
            }
            else {
                // TODO implement
                // _board.unMakeMove(move);
            }
        }
        
        // TODO implement
        // if(_board.inCheck()) {
        //     logger.log("turn has won!");
        // }
        _board.switchTurn();
    }

}


bool Game::isOver () const
{
    // TODO implement
    return false;
}