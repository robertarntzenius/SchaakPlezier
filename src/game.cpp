#include "game.h"
#include "log.h"

Game::Game(const char *FENString) 
    : logger(ChessLogger::getInstance()), _board(Board(FENString))
{
}

void Game::start()
{
    std::string input;

    std::vector<Move> moves;
    moves.reserve(64);

    Move move;

    do {
        _board.getPossibleMoves(moves);

        // Print board
        std::cout << _board;

        if (moves.empty()) {
            // Game over
            if (_board.inCheck()) {
                // Win for not-active player
                std::cout << ~_board.getActivePlayer() << " player won the game! \n";
                return;
            }

            // Stalemate
            std::cout << "Stalemate! \n";
            return;
        }

        // Parse move
        while (!parseMove(moves, input, move)) {
            std::cin >> input;
        };

        _board.doMove(move);
        
        //    _board.switchTurn();
    } while (std::cin >> input);
}

bool Game::parseMove(const std::vector<Move> &moves, std::string& userInput, Move &move) {
    if (userInput.size() != 4) {
        return false;
    }

    try {
        const Square fromSquare = stringSquareMap.at(userInput.substr(0, 2));
        const Square toSquare = stringSquareMap.at(userInput.substr(2, 2));

        auto isMove = [fromSquare, toSquare](const Move &move) {
            return ((move.fromSquare == fromSquare) && (move.targetSquare == toSquare));
        };

        auto it = std::find_if(moves.begin(), moves.end(), isMove);
        if (it == moves.end()) {
            std::cout << "Invalid move. Please input a legal move in the following notation:\n"
                      << " [square][square]\n"
                      << " for example: g2g4\n";
            return false;        }
        move = *it;

        if (move.isPromotion) {
            while (!parsePromotionMove(moves, it, userInput, move)) std::cin >> userInput;
        }

        return true;

    } catch (const std::exception &e) {
        std::cout << "Invalid input. Please input a valid move in the following notation:\n"
                  << " [square][square]\n"
                  << " for example: g2g4\n";
        return false;
    }



    return false;
}


bool Game::parsePromotionMove(const std::vector<Move> &moves, std::iterator<const Move *, std::vector<Move>> it, std::string &userInput, Move &move) {

    do {
        try {
            char promotionTypeChar;
            std::cout << "Please input a promotion piece type: (Q,N,B,R)";
            cin >> promotionTypeChar
            Piecetype type = charPiecetypeMap.at(promotionTypeChar);

            auto isPromotionMove = [fromSquare, toSquare, type](const Move &move) {
                return ((move.fromSquare == fromSquare)
                        && (move.targetSquare == toSquare)
                        && (move.promotionPiece == type));
            };

            auto promotionIt = std::find_if(it, moves.end(), isPromotionMove);
            if (promotionIt == moves.end()) {
                std::cout << "Invalid promotion piece type. Please input a legal move in the following notation:\n"
                          << " [square][square]\n"
                          << " for example: g2g4\n";
                return false;
            }
        } catch () {
            std::cout << "Invalid char. Not a piece type."
                      << "Please input a legal move in the following notation:\n"
                      << " [square][square]\n"
                      << " for example: g2g4\n";
            return false;
        }
    } while (true);
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