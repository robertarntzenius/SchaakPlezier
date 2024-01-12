//#include "board.h"
//
//void Board::generateRookMoves(std::vector<Move> &moveVector, Square fromSquare) const {
//#ifdef DEBUG
//    logger.logHeader("generateRookMoves()");
//#endif
//
//    const Bitboard occupied = colorBitboards[Black] | colorBitboards[White];
//
//    // Rook attacks
//    const Bitboard attacks = rookAttacksLookUp[fromSquare] & colorBitboards[~activePlayer];
//    std::vector<Square> toSquares = attacks.getIndices();
//    // logger.log(fromSquare, "Capture Knightmoves" );
//    // logger.log(knightAttacksLookUp[fromSquare]);
//    // logger.log(opponent);
//    // logger.log(attacks);
//
//
//    11111011
//
//    11011011
//
//
//
//    // 00p00R00
//
//
//
//    for (const auto &toSquare : toSquares) {
//        const Move move = MoveBuilder(Knight, fromSquare, toSquare)
//                .setCapture(pieceMaps[~activePlayer].at(toSquare), toSquare)
//                .build();
//        moveVector.push_back(move);
//    }
//
//    // Knight moves
//    const Bitboard moves = knightAttacksLookUp[fromSquare] & ~occupied;
//    toSquares.clear();
//    toSquares = moves.getIndices();
//
//    // logger.log(fromSquare, "Basic Knightmoves" );
//    // logger.log(knightAttacksLookUp[fromSquare]);
//    // logger.log(empty);
//    // logger.log(moves);
//
//    for (const auto &toSquare : toSquares) {
//        const Move move = MoveBuilder(Knight, fromSquare, toSquare).build();
//        moveVector.push_back(move);
//    }
//}