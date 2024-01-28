#include "board.h"

void Board::generateKingMoves(std::vector<Move> &moveVector, Square fromSquare) const {
    logger.logHeader("generateKingMoves", fromSquare);

    const Bitboard occupied = colorBitboards[Black] | colorBitboards[White];
    const Bitboard attacks = kingScopeLookUp[fromSquare] & colorBitboards[~activePlayer];

    // King attacks
    for (const auto &toSquare : attacks) {
        moveVector.emplace_back(
            MoveBuilder(King, fromSquare)
                .setTarget(toSquare)
                .setCapture(pieceMaps[~activePlayer].at(toSquare), toSquare)
                .build()
        );
        logger.debug(moveVector.back());

    }

    // King moves
    const Bitboard moves = kingScopeLookUp[fromSquare] & ~occupied;

    for (const auto &toSquare : moves) {
        moveVector.emplace_back(
            MoveBuilder(King, fromSquare)
                .setTarget(toSquare)
                .build()
        );
        logger.debug(moveVector.back());
    }

    // Castling
    switch (activePlayer) {
        case White:
            if (wKC) generateCastleMove(moveVector, wKingside); 
            if (wQC) generateCastleMove(moveVector, wQueenside); 
            break;
        case Black:
            if (bKC) generateCastleMove(moveVector, bKingside); 
            if (bQC) generateCastleMove(moveVector, bQueenside); 
            break;
        default:
            throw std::invalid_argument("invalid active player");
            break;
    }
}

void Board::generateCastleMove(std::vector<Move> &moveVector, CastlingSide side) const {
    logger.logHeader("generateCastleMove", activePlayer);

    static constexpr Bitboard wKingSideEmptySquares = Bitboard(0).set(f1).set(g1);
    static constexpr Bitboard wQueenSideEmptySquares = Bitboard(0).set(b1).set(c1).set(d1);
    static constexpr Bitboard bKingSideEmptySquares = Bitboard(0).set(f8).set(g8);
    static constexpr Bitboard bQueenSideEmptySquares = Bitboard(0).set(b8).set(c8).set(d8);

    const Bitboard occupied = colorBitboards[Black] | colorBitboards[White];
    const Bitboard opponentAttacks = getAttackedMask(~activePlayer);
    logger.verbose("opponentAttacks", opponentAttacks);

    switch (side) {
    case wKingside:
        if ((opponentAttacks.test(e1) || opponentAttacks.test(f1) || opponentAttacks.test(g1))) {
            logger.verbose("Attacked");
            return;
        };
        if (!(wKingSideEmptySquares & occupied).empty()) {
            logger.verbose("Not Empty");
            return;
        };
        moveVector.emplace_back(
            MoveBuilder(King, e1)
                .setTarget(g1)
                .setCastling()
                .build()
        );
        logger.debug(moveVector.back());
        break;
    case wQueenside:
        if ((opponentAttacks.test(c1) || opponentAttacks.test(d1) || opponentAttacks.test(e1))) {
            logger.verbose("Attacked");
            return;
        };
        if (!(wQueenSideEmptySquares & occupied).empty()) {
            logger.verbose("Not Empty");
            return;
        };
        moveVector.emplace_back(
            MoveBuilder(King, e1)
                .setTarget(c1)
                .setCastling()
                .build()
        );
        logger.debug(moveVector.back());
        break;
    case bKingside:
        if ((opponentAttacks.test(e8) ||opponentAttacks.test(f8) || opponentAttacks.test(g8))) {
            logger.verbose("Attacked");
            return;
        };
        if (!(bKingSideEmptySquares & occupied).empty()) {
            logger.verbose("Not Empty");
            return;
        };
        moveVector.emplace_back(
            MoveBuilder(King, e8)
                .setTarget(g8)
                .setCastling()
                .build()
        );
        logger.debug(moveVector.back());
        break;
    case bQueenside:
        if ((opponentAttacks.test(c8) || opponentAttacks.test(d8) || opponentAttacks.test(e8))) {
            logger.verbose("Attacked");
            return;
        };
        if (!(bQueenSideEmptySquares & occupied).empty()) {
            logger.verbose("Not Empty");
            return;
        };
        moveVector.emplace_back(
            MoveBuilder(King, e8)
                .setTarget(c8)
                .setCastling()
                .build()
        );
        logger.debug(moveVector.back());
        break;
    default:
        break;
    }
}