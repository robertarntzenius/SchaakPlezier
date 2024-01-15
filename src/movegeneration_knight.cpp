#include "board.h"

void Board::generateKnightMoves(std::vector<Move> &moveVector, Square fromSquare) const {
    #ifdef DEBUG
    logger.logHeader("generateKnightMoves()");
    #endif

    const Bitboard occupied = colorBitboards[Black] | colorBitboards[White];

    // Knight attacks
    const Bitboard attacks = knightScopeLookUp[fromSquare] & colorBitboards[~activePlayer];
    // logger.log(fromSquare, "Capture Knightmoves" );
    // logger.log(knightScopeLookUp[fromSquare]);
    // logger.log(opponent);
    // logger.log(attacks);

    for (const auto &toSquare : attacks) {
        const Move move = MoveBuilder(Knight, fromSquare, toSquare)
                .setCapture(pieceMaps[~activePlayer].at(toSquare), toSquare)
                .build();
        moveVector.push_back(move);
    }

    // Knight moves
    const Bitboard moves = knightScopeLookUp[fromSquare] & ~occupied;
    // logger.log(fromSquare, "Basic Knightmoves" );
    // logger.log(knightScopeLookUp[fromSquare]);
    // logger.log(empty);
    // logger.log(moves);

    for (const auto &toSquare : moves) {
        const Move move = MoveBuilder(Knight, fromSquare, toSquare).build();
        moveVector.push_back(move);
    }
}