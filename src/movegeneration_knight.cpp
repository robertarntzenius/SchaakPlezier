#include "board.h"

void Board::generateKnightMoves(std::vector<Move> &moveVector, Square fromSquare) const {
    #ifdef DEBUG
    logger.logHeader("generateKnightMoves()");
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
        #ifdef DEBUG
            logger.log(
                MoveBuilder(Knight, fromSquare)
                    .setTarget(toSquare)
                    .setCapture(pieceMaps[~activePlayer].at(toSquare), toSquare)
                    .build()
                );
        #endif
    }

    // Knight moves
    const Bitboard moves = knightScopeLookUp[fromSquare] & ~occupied;

    for (const auto &toSquare : moves) {
        moveVector.emplace_back(
            MoveBuilder(Knight, fromSquare)
                .setTarget(toSquare)
                .build()
        );
        #ifdef DEBUG
            logger.log(
                MoveBuilder(Knight, fromSquare)
                    .setTarget(toSquare)
                    .build()
                );
        #endif
    }
}