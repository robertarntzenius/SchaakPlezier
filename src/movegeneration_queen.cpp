#include "board.h"

void Board::generateQueenMoves(std::vector<Move> &moveVector, Square fromSquare) const {
#ifdef DEBUG
    logger.logHeader("generateQueenMoves()");
#endif
    const Bitboard occupied = colorBitboards[Black] | colorBitboards[White];

    Bitboard scope;

    // Queen attacks
    for (uint8_t direction = FirstOrthogonal; direction <= LastDiagonal; direction++) {
        Bitboard directionalScope = directionalLookUp[direction][fromSquare];
        Square nearestPieceLocation = NoSquare;

        switch (direction) {
            case North:
            case West:
            case NorthEast:
            case NorthWest:
                nearestPieceLocation = (directionalScope & occupied).getLowestSetBit();
                scope = scope | directionalScope.resetUpperBits(nearestPieceLocation);
                break;
            case South:
            case East:
            case SouthEast:
            case SouthWest:
                nearestPieceLocation = (directionalScope & occupied).getHighestSetBit();
                scope = scope | directionalScope.resetLowerBits(nearestPieceLocation);
                break;
            default:
                throw std::invalid_argument("Direction of queen should be diagonal or orthogonal. Invalid direction received.");
        }

        if (colorBitboards[~activePlayer].test(nearestPieceLocation)) {
            const Piecetype capturePiece = pieceMaps[~activePlayer].at(nearestPieceLocation);
            // add capture move
            moveVector.emplace_back(
                    MoveBuilder(Queen, fromSquare)
                        .setTarget(nearestPieceLocation)
                        .setCapture(capturePiece, nearestPieceLocation)
                        .build()
            );
            #ifdef DEBUG
                logger.log(
                    MoveBuilder(Queen, fromSquare)
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
                MoveBuilder(Queen, fromSquare)
                    .setTarget(toSquare)
                    .build()
        );
        #ifdef DEBUG
            logger.log(
                MoveBuilder(Queen, fromSquare)
                    .setTarget(toSquare)
                    .build()
                );
        #endif
    }
}