#include "board.h"

void Board::generateKnightMoves(std::vector<Move> &moveVector, Square fromSquare) const {
    #ifdef DEBUG
    logger.logHeader("generateKnightMoves", fromSquare);
    #endif

    const Bitboard occupied = colorBitboards[Black] | colorBitboards[White];
    const Bitboard attacks = knightScopeLookUp[fromSquare] & colorBitboards[~activePlayer];

    // Knight attacks
    for (const auto &toSquare : attacks) {
        moveVector.emplace_back(
            MoveBuilder(Knight, fromSquare)
                .setTarget(toSquare)
                .setCapture(pieceMaps[~activePlayer].at(toSquare), toSquare)
                .build()
        );
        logger.debug(moveVector.back());
    }

    // Knight moves
    const Bitboard moves = knightScopeLookUp[fromSquare] & ~occupied;

    for (const auto &toSquare : moves) {
        moveVector.emplace_back(
            MoveBuilder(Knight, fromSquare)
                .setTarget(toSquare)
                .build()
        );
        logger.debug(moveVector.back());

    }
}