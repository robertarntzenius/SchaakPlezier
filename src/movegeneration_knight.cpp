#include "board.h"

using namespace MoveGeneration;

void Board::generateKnightMoves(std::vector<std::unique_ptr<Move>> &moveVector) const {
    #ifdef DEBUG
    logger.logHeader("generateKnightMoves()");
    #endif

    const Bitboard player = (activePlayer == White) ? colorBitboards[White] : colorBitboards[Black];
    const Bitboard opponent = (activePlayer == White) ? colorBitboards[Black] : colorBitboards[White];
    const Bitboard empty = colorBitboards[Black] | colorBitboards[White];
    
    // for every piece
    for (const auto &squarePiecetypePair: pieceMaps[activePlayer]) {
        const Square &fromSquare = squarePiecetypePair.first;
        const Piecetype &type = squarePiecetypePair.second;

        // for every knight
        if (type != Knight) {
            continue;
        }

        // Knight attacks
        const Bitboard attacks = knightAttacksLookUp[fromSquare] & opponent;
        std::vector<Square> toSquares = attacks.getIndices();

        for (const auto &toSquare : toSquares) {
            const Move move = createCapture(fromSquare, toSquare, Knight, pieceMaps[~activePlayer].at(toSquare));
            moveVector.emplace_back(std::make_unique<Move>(move));
        }

        // Knight moves
        const Bitboard moves = knightAttacksLookUp[fromSquare] & empty;
        toSquares.clear();
        toSquares = moves.getIndices();

        for (const auto &toSquare : toSquares) {
            const Move move = createMove(fromSquare, toSquare, Knight);
            moveVector.emplace_back(std::make_unique<Move>(move));
        }
    }
}