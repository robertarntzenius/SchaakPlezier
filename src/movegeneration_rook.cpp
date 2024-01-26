#include "board.h"

void Board::generateRookMoves(std::vector<Move> &moveVector, Square fromSquare) const {
#ifdef DEBUG
    logger.logHeader("generateRookMoves()");
#endif

    const Bitboard occupied = colorBitboards[Black] | colorBitboards[White];

    Bitboard scope;

    // Rook attacks
    for (uint8_t direction = FirstOrthagonal; direction <= LastOrthogonal; direction++) {
        Bitboard directionalScope = directionalLookUp[direction][fromSquare];
        Square nearestPieceLocation = NoSquare;

        switch (direction) {
            case North:
            case West:
                nearestPieceLocation = (directionalScope & occupied).getLowestSetBit();
                scope = scope | directionalScope.resetUpperBits(nearestPieceLocation);
                break;
            case South:
            case East:
                nearestPieceLocation = (directionalScope & occupied).getHighestSetBit();
                scope = scope | directionalScope.resetLowerBits(nearestPieceLocation);
                break;
            default:
                throw std::invalid_argument("Direction of rook should be orthagonal. Invalid direction received.");
        }

        if (colorBitboards[~activePlayer].test(nearestPieceLocation)) {
            const Piecetype capturePiece = pieceMaps[~activePlayer].at(nearestPieceLocation);
            // add capture move
            moveVector.emplace_back(
                    MoveBuilder(Rook, fromSquare)
                        .setTarget(nearestPieceLocation)
                        .setCapture(capturePiece, nearestPieceLocation)
                        .build()
            );
        }
    }

    for (const auto& toSquare : scope) {
        // add normal moves
        moveVector.emplace_back(
                MoveBuilder(Rook, fromSquare)
                        .setTarget(toSquare)
                        .build()
        );
    }
}