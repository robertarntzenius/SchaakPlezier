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

        toSquares.clear();
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

//    Piecetype playerPawnType = (activePlayer == Color::White) ? wPawn : bPawn;
//    Piecetype opponentPawnType = (activePlayer == Color::White) ? bPawn : wPawn;
//    const Bitboard *playerPawns = (activePlayer == Color::White) ? &wpawns : &bpawns;
//    const Bitboard *opponent = (activePlayer == Color::White) ? &black : &white;
//    const Bitboard empty = Bitboard(black | white).flip();
//
//
//    // Generate Capture Moves
//    for (Square fromSquare : getIndices(*playerPawns)) {
//        // doing the comparison here skips the for loop for many pawns
//        Bitboard validAttacks = (PawnAttacks[color][fromSquare] & *opponent);
//        for (Square toSquare : getIndices(validAttacks)) {
//#ifdef DEBUG
//            Move move(fromSquare, toSquare, playerPawnType, findPiece(toSquare));
//            logger.log("CAPTURE");
//            logger.log(move);
//#endif
//            pawnMoves.emplace_back(fromSquare, toSquare, playerPawnType, findPiece(toSquare));
//        }
//
//        // enPassantSquare
//        // cant use validAttacks because the enPassantSquare square is empty by definition
//        if ((enPassantSquare != NoSquare) && (PawnAttacks[color][fromSquare].test(enPassantSquare))) {
//#ifdef DEBUG
//            Move move(fromSquare, enPassantSquare, playerPawnType, opponentPawnType);
//            logger.log("ENPASSANT");
//            logger.log(move);
//#endif
//            Square capturedPieceSquare = (activePlayer == White) ? intToSquare(enPassantSquare + BOARD_DIMENSIONS) : intToSquare(enPassantSquare - BOARD_DIMENSIONS);
//            pawnMoves.emplace_back(fromSquare, enPassantSquare, playerPawnType, findPiece(capturedPieceSquare));
//            // TODO: make sure to handle this move correctly when actually making it
//        }
//    }
//
//    // Generate Push moves
//    for (Square fromSquare : getIndices(*playerPawns)) { // for every pawn
//        for (Square toSquare : getIndices(PawnPushes[color][fromSquare])) { // for every attacked square of that pawn
//
//            // Double pushes
//            if ( (abs(fromSquare - toSquare) == 2 * BOARD_DIMENSIONS)
//                 && (empty.test(intToSquare((toSquare+fromSquare)/2))) // if one square in front is empty
//                 && (empty.test(toSquare)) )
//            {
//#ifdef DEBUG
//                Move move(fromSquare, toSquare, playerPawnType, NoType);
//                logger.log("DOUBLE PUSH");
//                logger.log(move);
//#endif
//                pawnMoves.emplace_back(fromSquare, toSquare, playerPawnType, NoType, (intToSquare((toSquare+fromSquare)/2)));
//
//                // TODO set enpassant square or at least pass it along with the move?
//            }
//
//                // Single pushes
//            else if ( (abs(fromSquare - toSquare) == BOARD_DIMENSIONS)
//                      && empty.test(toSquare))
//            {
//#ifdef DEBUG
//                Move move(fromSquare, toSquare, playerPawnType, NoType);
//                logger.log("SINGLE PUSH");
//                logger.log(move);
//#endif
//                pawnMoves.emplace_back(fromSquare, toSquare, playerPawnType, NoType);
//            }
//        }
//    }
//    return pawnMoves;




