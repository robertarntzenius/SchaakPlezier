#include "board.h"


void Board::generatePawnMoves(std::vector<std::unique_ptr<Move>> &moveVector) const {
#ifdef DEBUG
    logger.logHeader("generatePawnMoves()");
#endif

    const Bitboard occupied = (colorBitboards[White] | colorBitboards[Black]);

    for (const auto &squarePiecetypePair: pieceMaps[activePlayer]) {
        const Square fromSquare = squarePiecetypePair.first;
        const Piecetype type = squarePiecetypePair.second;

        if (type != Pawn) {
            continue;
        }

        const Bitboard pushes = pawnPushLookUp[activePlayer][fromSquare];

        std::vector<Square> toSquares = pushes.getIndices();
        for (const auto &toSquare : toSquares) {
            if (occupied.test(toSquare)) {
                continue;
            }
            
            if (abs(toSquare - fromSquare) == 2 * BOARD_DIMENSIONS) {
                // Double push (check in between)
                const Square newEnPassantSquare = intToSquare((toSquare + fromSquare) / 2);

                if (occupied.test(newEnPassantSquare)) {
                    continue;
                }

                const DoublePawnMove move = {{fromSquare, toSquare, Move::DoublePawn}, newEnPassantSquare};
                moveVector.emplace_back(std::make_unique<DoublePawnMove>(move));

            } else {
                // Single push
                const Move move = {fromSquare, toSquare, Move::Basic};
                moveVector.emplace_back(std::make_unique<Move>(move));
            }
        }

        const Bitboard attacks = pawnAttackLookUp[activePlayer][fromSquare];

        toSquares = (attacks & colorBitboards[invertColor(activePlayer)]).getIndices();
        for (const auto &toSquare : toSquares) {
            const CaptureMove move = {{fromSquare, toSquare, Move::Capture}, toSquare};
            moveVector.emplace_back(std::make_unique<CaptureMove>(move));
        }

        if (attacks.test(enPassantSquare)) {
            const Square captureSquare = intToSquare(fromSquare + (2 * (enPassantSquare - fromSquare)));
            const CaptureMove move = {{fromSquare, captureSquare, Move::Capture}, enPassantSquare};

            moveVector.emplace_back(std::make_unique<CaptureMove>(move));
        }
    }
}