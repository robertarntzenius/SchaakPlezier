#include "game.h"
#include "log.h"

Game::Game(const char *FENString) 
    : logger(ChessLogger::getInstance()), _board(Board(FENString))
{
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
    std::vector<Move> moves;
    _board.getPossibleMoves(moves);

    logger.logHeader("moves in game.test");
    for (const auto &move : moves)
    {
        logger.essential(move);
    }

    Bitboard lonelyRook{};

    Square rookSquare = f4;

    lonelyRook.set(rookSquare);

    logger.essential(lonelyRook,
               "\n", Bitboard().set().resetLowerBits(rookSquare),
               "\n", Bitboard().set().resetUpperBits(rookSquare));

//            bool moveIsLegal = _board.makeMove(move);
//            if (moveIsLegal) {
//                break;
//            }
//            else {
//                // TODO implement
//                // _board.unMakeMove(move);
//            }
    

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