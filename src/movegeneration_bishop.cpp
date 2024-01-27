#include "board.h"

void Board::generateBishopMoves(std::vector<Move> &moveVector, Square fromSquare) const {
#ifdef DEBUG
    logger.logHeader("generateBishopMoves()");
#endif
    const Bitboard occupied = colorBitboards[Black] | colorBitboards[White];

    Bitboard scope;

    // Bishop attacks
    for (uint8_t direction = FirstDiagonal; direction <= LastDiagonal; direction++) {
        Bitboard directionalScope = directionalLookUp[direction][fromSquare];
        Square nearestPieceLocation = NoSquare;

        switch (direction) {
            case NorthEast:
            case NorthWest:
                nearestPieceLocation = (directionalScope & occupied).getLowestSetBit();
                scope = scope | directionalScope.resetUpperBits(nearestPieceLocation);
                break;
            case SouthEast:
            case SouthWest:
                nearestPieceLocation = (directionalScope & occupied).getHighestSetBit();
                scope = scope | directionalScope.resetLowerBits(nearestPieceLocation);
                break;
            default:
                throw std::invalid_argument("Direction of bishop should be diagonal. Invalid direction received.");
        }

        if (colorBitboards[~activePlayer].test(nearestPieceLocation)) {
            const Piecetype capturePiece = pieceMaps[~activePlayer].at(nearestPieceLocation);
            // add capture move
            moveVector.emplace_back(
                MoveBuilder(Bishop, fromSquare)
                    .setTarget(nearestPieceLocation)
                    .setCapture(capturePiece, nearestPieceLocation)
                    .build()
            );
            #ifdef DEBUG
                logger.log(
                    MoveBuilder(Bishop, fromSquare)
                        .setTarget(nearestPieceLocation)
                        .setCapture(capturePiece, nearestPieceLocation)
                        .build()
                    );
            #endif
        }
    }

    for (const auto& toSquare : scope) {
        // add normal moves
        moveVector.emplace_back(
            MoveBuilder(Bishop, fromSquare)
                .setTarget(toSquare)
                .build()
        );
        #ifdef DEBUG
            logger.log(
                MoveBuilder(Bishop, fromSquare)
                    .setTarget(toSquare)
                    .build()
                );
        #endif
    }
}