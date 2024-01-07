#include "board.h"

using namespace MoveGeneration;

void Board::generatePawnMoves(std::vector<std::unique_ptr<Move>> &moveVector) const {
    #ifdef DEBUG
    logger.logHeader("generatePawnMoves()");
    #endif
    // for every piece
    for (const auto &squarePiecetypePair: pieceMaps[activePlayer]) {
        const Square &fromSquare = squarePiecetypePair.first;
        const Piecetype &type = squarePiecetypePair.second;

        // for every pawn
        if (type != Pawn) {
            continue;
        }
        generatePawnPushes(moveVector, fromSquare);
        generatePawnCaptures(moveVector, fromSquare);
    }
}

void Board::generatePawnPushes(std::vector<std::unique_ptr<Move>> &moveVector, const Square &fromSquare) const {
    const Bitboard occupied = (colorBitboards[White] | colorBitboards[Black]);
    const Bitboard finalRank = (activePlayer == White) ? MaskGeneration::computeRankMask(Rank8) : MaskGeneration::computeRankMask(Rank1);
    const Bitboard pushes = pawnPushLookUp[activePlayer][fromSquare];
    std::vector<Square> toSquares = pushes.getIndices();

    for (const auto &toSquare : toSquares) {
        if (occupied.test(toSquare)) {
            continue;
        }

        // Double push (check in between)
        if (abs(toSquare - fromSquare) == 2 * BOARD_DIMENSIONS) {
            const Square newEnPassantSquare = intToSquare((toSquare + fromSquare) / 2);

            if (occupied.test(newEnPassantSquare)) {
                continue;
            }

            const Move move = createDoublePawnMove(fromSquare, toSquare, newEnPassantSquare);
            moveVector.emplace_back(std::make_unique<Move>(move));
        }

        // Single push
        else {
            if (finalRank.test(toSquare)) {
                for (Piecetype promotionType : promotionPiecetypes) {
                    const Move move = createPromotionMove(fromSquare, toSquare, promotionType);
                    moveVector.emplace_back(std::make_unique<Move>(move));
                }
            } else {
                const Move move = createMove(fromSquare, toSquare, Pawn);
                moveVector.emplace_back(std::make_unique<Move>(move));
            }
        }
    }
}

void Board::generatePawnCaptures(std::vector<std::unique_ptr<Move>> &moveVector, const Square &fromSquare) const {
    const Bitboard finalRank = (activePlayer == White) ? MaskGeneration::computeRankMask(Rank8) : MaskGeneration::computeRankMask(Rank1);
    const Bitboard attacks = pawnAttackLookUp[activePlayer][fromSquare];
    std::vector<Square> toSquares = (attacks & colorBitboards[invertColor(activePlayer)]).getIndices();

    for (const auto &toSquare : toSquares) {
        const Piecetype capturePiecetype = pieceMaps[~activePlayer].at(toSquare);

        // Promotion capture
        if (finalRank.test(toSquare)) {
            for (Piecetype promotionType : promotionPiecetypes) {
                const Move move = createPromotionCapture(fromSquare,toSquare, capturePiecetype, promotionType);
                moveVector.emplace_back(std::make_unique<Move>(move));
            }

        // Normal capture
        } else {
            const Move move = createCapture(fromSquare, toSquare, Pawn, capturePiecetype);
            moveVector.emplace_back(std::make_unique<Move>(move));
        }
    }

    // Enpassant capture
    if (attacks.test(enPassantSquare)) {
        const Square captureSquare = rankFileToSquare(squareToRank(fromSquare), squareToFile(enPassantSquare));
        const Piecetype capturePiecetype = pieceMaps[~activePlayer].at(captureSquare);
        const Move move = createEnPassantCapture(fromSquare, enPassantSquare, captureSquare, capturePiecetype);
        moveVector.emplace_back(std::make_unique<Move>(move));
    }
}