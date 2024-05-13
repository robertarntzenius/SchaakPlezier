#include "humanplayer.h"


Move HumanPlayer::decideOnMove( [[maybe_unused]] Board boardCopy, const std::vector<Move> &moves) {
    std::string input;
    size_t playerMoveIndex = 0;
    ChessLogger &logger = ChessLogger::getInstance("HumanPlayer.log");


    // FIXME
    do {
        std::cout << "Move: ";
        std::cin >> input;
        if (input == "quit") {
            logger.verbose("quit");
//            return SIZE_MAX;
        }
        if (input.starts_with('q') || input.starts_with('Q')) {
            std::cout << "Did you intend to quit the game? (y/n)\n";
            std::cin >> input;
            if (input == "y") {
                logger.verbose("quit");
//                return SIZE_MAX;
            }
        }
    } while (!parseMove(moves, input, playerMoveIndex));

    return moves[playerMoveIndex];
}


json HumanPlayer::getSettings() {
    json settings = {
        {"playerType", playerTypeStringMap.at(Human)}
    };
    return settings;
}

/* private: */
bool HumanPlayer::parseMove(const std::vector<Move> &moves, std::string& userInput, size_t &moveIndex) {
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

        auto move_it = std::find_if(moves.begin(), moves.end(), isMove);
        if (move_it == moves.end()) {
            std::cout << "Invalid move. Please input a legal move in the following notation:\n"
                      << " [square][square]\n"
                      << " for example: g2g4\n";
            return false;
        }

        moveIndex = std::distance(moves.begin(), move_it);

        // Set move to move found in vector
        Move move = *move_it;

        if (move.isPromotion) {
            // If promotion move, ask for promotion type and set accordingly
            bool valid = false;

            do {
                char promotionTypeChar = 0;
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
            
            Piecetype promotionPiecetype = move.promotionPiece;
            auto isPromotionMove = [fromSquare, toSquare, promotionPiecetype](const Move &move) {
                return (   (move.fromSquare == fromSquare) 
                        && (move.targetSquare == toSquare) 
                        && (move.promotionPiece == promotionPiecetype));
            };

            auto promo_it = std::find_if(moves.begin(), moves.end(), isPromotionMove);
            moveIndex = std::distance(moves.begin(), promo_it);
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