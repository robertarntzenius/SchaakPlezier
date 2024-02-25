#include "board.h"

//using namespace MoveGeneration;
void Board::generatePawnMoves(std::vector<Move> &moveVector, Square fromSquare) const {
    logger.logHeader("generatePawnMoves", fromSquare);
    generatePawnCaptures(moveVector, fromSquare);
    generatePawnPushes(moveVector, fromSquare);
}

void Board::generatePawnPushes(std::vector<Move> &moveVector, Square fromSquare) const {
    const Bitboard occupied = (colorBitboards[White] | colorBitboards[Black]);
    const Bitboard pushes = pawnPushLookUp[boardState.activePlayer][fromSquare];

    // Only check available squares
    for (const auto &toSquare : (pushes & ~occupied)) {
        // Double push (check in between)
        if (abs(toSquare - fromSquare) == 2 * BOARD_DIMENSIONS) {
            const Square newEnPassantSquare = intToSquare((toSquare + fromSquare) / 2);

            if (occupied.test(newEnPassantSquare)) {
                return;
            }

            moveVector.emplace_back(
                MoveBuilder(Pawn, fromSquare)
                    .setTarget(toSquare)
                    .setEnPassant(newEnPassantSquare)
                    .build()
            );
            logger.debug(moveVector.back());

        }

        // Single push
        else {
            if (finalRank[boardState.activePlayer].test(toSquare)) {
                for (Piecetype promotionType : promotionPiecetypes) {
                    moveVector.emplace_back(
                        MoveBuilder(Pawn, fromSquare)
                            .setTarget(toSquare)
                            .setPromotion(promotionType)
                            .build()
                    );
                    logger.debug(moveVector.back());
                }
            }
            else {
                moveVector.emplace_back(
                    MoveBuilder(Pawn, fromSquare)
                        .setTarget(toSquare)
                        .build()
                );
                logger.debug(moveVector.back());
            }
        }
    }
}

void Board::generatePawnCaptures(std::vector<Move> &moveVector, Square fromSquare) const {
    const Bitboard attacks = pawnAttackLookUp[boardState.activePlayer][fromSquare];

    for (const auto &toSquare : (attacks & colorBitboards[~boardState.activePlayer])) {
        const Piecetype capturePiecetype = pieceMaps[~boardState.activePlayer].at(toSquare);

        // Promotion capture
        if (finalRank[boardState.activePlayer].test(toSquare)) {
            for (Piecetype promotionType : promotionPiecetypes) {
                moveVector.emplace_back(
                    MoveBuilder(Pawn, fromSquare)
                        .setTarget(toSquare)
                        .setCapture(capturePiecetype, toSquare)
                        .setPromotion(promotionType)
                        .build()
                );
                logger.debug(moveVector.back());

            }

        // Normal capture
        } else {
            moveVector.emplace_back(
                MoveBuilder(Pawn, fromSquare)
                    .setTarget(toSquare)
                    .setCapture(capturePiecetype, toSquare)
                    .build()
            );
            logger.debug(moveVector.back());


        }
    }

    // Enpassant capture
    if (attacks.test(boardState.enPassantSquare)) {
        const Square captureSquare = rankFileToSquare(squareToRank(fromSquare), squareToFile(boardState.enPassantSquare));
        const Piecetype capturePiecetype = pieceMaps[~boardState.activePlayer].at(captureSquare);
        moveVector.emplace_back(
            MoveBuilder(Pawn, fromSquare)
                .setTarget(boardState.enPassantSquare)
                .setCapture(capturePiecetype, captureSquare)
                .build()
        );
        logger.debug(moveVector.back());

    }
}