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

    // Opening messages

    // Clear screen
    std::cout << "\033[2J\033[H";
    std::cout << "Starting new game. " << _board.getActivePlayer() << " to play.\n"
              << "Type \"quit\" to end the game. Enter moves by typing a square your "
              << "piece occupies followed by the square you want to move it to. (e2e4)\n\n"
              << "Enjoy!\n";

    while (true) {
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
        do {
            std::cout << "Move: ";
            std::cin >> input;
            if (input == "quit") {
                return;
            }
            if (input.starts_with('q') || input.starts_with('Q')) {
                std::cout << "Did you intend to quit the game? (y/n)\n";
                std::cin >> input;
                if (input == "y") {
                    return;
                }
            }
        } while (!parseMove(moves, input, move));

        _board.doMove(move);

        std::cout << "\033[2J\033[H";
    }


}

bool Game::parseMove(const std::vector<Move> &moves, std::string& userInput, Move &move) {
    if (userInput.size() != 4) {
        std::cout << "Invalid input. Please input a valid move in the following notation:\n"
                  << " [square][square]\n"
                  << " for example: g2g4\n";
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
            return false;
        }

        // Set move to move found in vector
        move = *it;

        if (move.isPromotion) {
            // If promotion move, ask for promotion type and set accordingly
            bool valid = false;

            do {
                char promotionTypeChar;
                std::cout << "Please input a promotion piece type: (Q,N,B,R): ";
                std::cin >> promotionTypeChar;

                switch (promotionTypeChar) {
                    case 'q': case 'Q': case 'n': case 'N':
                    case 'b': case 'B': case 'r': case 'R':
                        move.promotionPiece = charPiecetypeMap.at(promotionTypeChar);
                        valid = true;
                        break;
                    default:
                        std::cout << "Invalid char. Not a piece type.\n";
                        break;
                }
            } while (!valid);
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