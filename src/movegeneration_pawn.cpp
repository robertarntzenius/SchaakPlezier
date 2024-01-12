#include "board.h"

//using namespace MoveGeneration;

void Board::generatePawnPushes(std::vector<Move> &moveVector, Square fromSquare) const {
    const Bitboard occupied = (colorBitboards[White] | colorBitboards[Black]);
    const Bitboard pushes = pawnPushLookUp[activePlayer][fromSquare];

    // Only check available squares
    const std::vector<Square> toSquares = (pushes & ~occupied).getIndices();

    for (const auto &toSquare : toSquares) {
        // Double push (check in between)
        if (abs(toSquare - fromSquare) == 2 * BOARD_DIMENSIONS) {
            const Square newEnPassantSquare = intToSquare((toSquare + fromSquare) / 2);

            if (occupied.test(newEnPassantSquare)) {
                return;
            }

            const Move move =
                    MoveBuilder(Pawn, fromSquare, toSquare)
                    .setEnPassant(newEnPassantSquare)
                    .build();
            moveVector.push_back(move);
        }

        // Single push
        else {
            if (finalRank[activePlayer].test(toSquare)) {
                for (Piecetype promotionType : promotionPiecetypes) {
                    const Move move =
                            MoveBuilder(Pawn, fromSquare, toSquare)
                            .setPromotion(promotionType)
                            .build();

                    moveVector.push_back(move);
                }
            } else {
                const Move move =
                        MoveBuilder(Pawn, fromSquare, toSquare)
                                .build();

                moveVector.push_back(move);
            }
        }
    }
}

void Board::generatePawnCaptures(std::vector<Move> &moveVector, Square fromSquare) const {
    const Bitboard attacks = pawnAttackLookUp[activePlayer][fromSquare];

    std::vector<Square> toSquares = (attacks & colorBitboards[~activePlayer]).getIndices();

    for (const auto &toSquare : toSquares) {
        const Piecetype capturePiecetype = pieceMaps[~activePlayer].at(toSquare);

        // Promotion capture
        if (finalRank[activePlayer].test(toSquare)) {
            for (Piecetype promotionType : promotionPiecetypes) {
                const Move move =
                        MoveBuilder(Pawn, fromSquare, toSquare)
                        .setCapture(capturePiecetype, toSquare)
                        .setPromotion(promotionType)
                        .build();

                moveVector.push_back(move);
            }

        // Normal capture
        } else {
            const Move move =
                    MoveBuilder(Pawn, fromSquare, toSquare)
                            .setCapture(capturePiecetype, toSquare)
                            .build();
            moveVector.push_back(move);
        }
    }

    // Enpassant capture
    if (attacks.test(enPassantSquare)) {
        const Square captureSquare = rankFileToSquare(squareToRank(fromSquare), squareToFile(enPassantSquare));
        const Piecetype capturePiecetype = pieceMaps[~activePlayer].at(captureSquare);
        const Move move = MoveBuilder(Pawn, fromSquare, enPassantSquare)
                                .setCapture(capturePiecetype, captureSquare)
                                .build();

        moveVector.push_back(move);
    }
}