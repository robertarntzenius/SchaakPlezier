#include "game.h"
#include "log.h"

Game::Game(const char *FENString) 
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
    std::vector<std::unique_ptr<Move>> moves;
    _board.getPossibleMoves(moves);

    logger.logHeader("moves in game.test()");

    for (const auto &move : moves)
    {
        switch (move->type) {
            case Move::Basic:
                logger.log("Basic Move: ");
                logger.log(*move);
                break;
            case Move::DoublePawn:
                logger.log("Double Pawn: ");
                logger.log(*move);
                break;
            case Move::Capture:
                logger.log("Capture: ");
                logger.log(*move);
                break;
            default:
                break;
        }
//            bool moveIsLegal = _board.makeMove(move);
//            if (moveIsLegal) {
//                break;
//            }
//            else {
//                // TODO implement
//                // _board.unMakeMove(move);
//            }
    }

// TODO implement
// if(_board.inCheck()) {
//     logger.log("turn has won!");
// }
//        _board.switchTurn();


}


bool Game::isOver () const
{
    // TODO implement
    return false;
}