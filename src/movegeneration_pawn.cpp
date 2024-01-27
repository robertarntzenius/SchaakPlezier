#include "board.h"

//using namespace MoveGeneration;
void Board::generatePawnMoves(std::vector<Move> &moveVector, Square fromSquare) const {
#ifdef DEBUG
    logger.logHeader("generatePawnMoves()");
#endif
    generatePawnCaptures(moveVector, fromSquare);
    generatePawnPushes(moveVector, fromSquare);
}

void Board::generatePawnPushes(std::vector<Move> &moveVector, Square fromSquare) const {
    const Bitboard occupied = (colorBitboards[White] | colorBitboards[Black]);
    const Bitboard pushes = pawnPushLookUp[activePlayer][fromSquare];

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
            #ifdef DEBUG
                logger.log(
                    MoveBuilder(Pawn, fromSquare)
                        .setTarget(toSquare)
                        .setEnPassant(newEnPassantSquare)
                        .build()
                );
            #endif
        }

        // Single push
        else {
            if (finalRank[activePlayer].test(toSquare)) {
                for (Piecetype promotionType : promotionPiecetypes) {
                    moveVector.emplace_back(
                        MoveBuilder(Pawn, fromSquare)
                            .setTarget(toSquare)
                            .setPromotion(promotionType)
                            .build()
                    );
                #ifdef DEBUG
                    logger.log(
                        MoveBuilder(Pawn, fromSquare)
                            .setTarget(toSquare)
                            .setPromotion(promotionType)
                            .build()
                        );
                #endif
                }
            } else {
                moveVector.emplace_back(
                    MoveBuilder(Pawn, fromSquare)
                        .setTarget(toSquare)
                        .build()
                );
                #ifdef DEBUG
                    logger.log(
                        MoveBuilder(Pawn, fromSquare)
                            .setTarget(toSquare)
                            .build()
                        );
                #endif
            }
        }
    }
}

void Board::generatePawnCaptures(std::vector<Move> &moveVector, Square fromSquare) const {
    const Bitboard attacks = pawnAttackLookUp[activePlayer][fromSquare];

    for (const auto &toSquare : (attacks & colorBitboards[~activePlayer])) {
        const Piecetype capturePiecetype = pieceMaps[~activePlayer].at(toSquare);

        // Promotion capture
        if (finalRank[activePlayer].test(toSquare)) {
            for (Piecetype promotionType : promotionPiecetypes) {
                moveVector.emplace_back(
                    MoveBuilder(Pawn, fromSquare)
                        .setTarget(toSquare)
                        .setCapture(capturePiecetype, toSquare)
                        .setPromotion(promotionType)
                        .build()
                );
                #ifdef DEBUG
                    logger.log(
                        MoveBuilder(Pawn, fromSquare)
                            .setTarget(toSquare)
                            .setCapture(capturePiecetype, toSquare)
                            .setPromotion(promotionType)
                            .build()
                        );
                #endif
            }

        // Normal capture
        } else {
            moveVector.emplace_back(
                MoveBuilder(Pawn, fromSquare)
                    .setTarget(toSquare)
                    .setCapture(capturePiecetype, toSquare)
                    .build()
            );
            #ifdef DEBUG
                logger.log(
                    MoveBuilder(Pawn, fromSquare)
                        .setTarget(toSquare)
                        .setCapture(capturePiecetype, toSquare)
                        .build()
                    );
            #endif

        }
    }

    // Enpassant capture
    if (attacks.test(enPassantSquare)) {
        const Square captureSquare = rankFileToSquare(squareToRank(fromSquare), squareToFile(enPassantSquare));
        const Piecetype capturePiecetype = pieceMaps[~activePlayer].at(captureSquare);
        moveVector.emplace_back(
            MoveBuilder(Pawn, fromSquare)
                .setTarget(enPassantSquare)
                .setCapture(capturePiecetype, captureSquare)
                .build()
        );
        #ifdef DEBUG
            logger.log(
                MoveBuilder(Pawn, fromSquare)
                    .setTarget(enPassantSquare)
                    .setCapture(capturePiecetype, captureSquare)
                    .build()
                );
        #endif
    }
}