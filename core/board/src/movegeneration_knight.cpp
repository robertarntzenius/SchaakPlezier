#include "board.h"

void Board::generateKnightMoves(std::vector<Move> &moveVector, Square fromSquare) const {
    logger.logHeader("generateKnightMoves", fromSquare);

    const Bitboard occupied = colorBitboards[Black] | colorBitboards[White];
    const Bitboard attacks = knightScopeLookUp[fromSquare] & colorBitboards[~boardState.activePlayer];

    // Knight attacks
    for (const auto &toSquare : attacks) {
        moveVector.emplace_back(
            MoveBuilder(Knight, fromSquare)
                .setTarget(toSquare)
                .setCapture(pieceMaps[~boardState.activePlayer].at(toSquare), toSquare)
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