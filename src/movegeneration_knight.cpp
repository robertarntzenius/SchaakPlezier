#include "board.h"

void Board::generateKnightMoves(std::vector<Move> &moveVector, Square fromSquare) const {
    #ifdef DEBUG
    logger.logHeader("generateKnightMoves()");
    #endif

    const Bitboard occupied = colorBitboards[Black] | colorBitboards[White];

    // Knight attacks
    const Bitboard attacks = knightScopeLookUp[fromSquare] & colorBitboards[~activePlayer];

    for (const auto &toSquare : attacks) {
        moveVector.emplace_back(
            MoveBuilder(Knight, fromSquare)
            .setTarget(toSquare)
            .setCapture(pieceMaps[~activePlayer].at(toSquare), toSquare)
            .build()
        );
    }

    // Knight moves
    const Bitboard moves = knightScopeLookUp[fromSquare] & ~occupied;

    for (const auto &toSquare : moves) {
        moveVector.emplace_back(
            MoveBuilder(Knight, fromSquare)
            .setTarget(toSquare)
            .build()
        );
    }
}