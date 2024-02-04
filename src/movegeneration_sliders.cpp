#include "board.h"

void Board::generateSliderMoves(std::vector<Move> &moveVector, Square fromSquare, Piecetype pieceType) const {
    logger.logHeader("generateSliderMoves", fromSquare);
    const Bitboard occupied = colorBitboards[Black] | colorBitboards[White];
    uint8_t firstDirection, lastDirection;

    Bitboard scope;
    switch (pieceType) {
        case Queen:
            firstDirection = FirstOrthogonal;
            lastDirection = LastDiagonal;
        break;

        case Bishop:
            firstDirection = FirstDiagonal;
            lastDirection = LastDiagonal;
        break;

        case Rook:
            firstDirection = FirstOrthogonal;
            lastDirection = LastOrthogonal;
        break;
        
        default:
            throw std::invalid_argument("Invalid Piecetype, generateSliderMoves() should be called with a slider");
        break;
    }

    // Slider attacks
    for (uint8_t direction = firstDirection; direction <= lastDirection; direction++) {
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
                throw std::invalid_argument("Direction of Slider should be diagonal or orthogonal. Invalid direction received.");
        }
        

        if (colorBitboards[~activePlayer].test(nearestPieceLocation)) {
            const Piecetype capturePiece = pieceMaps[~activePlayer].at(nearestPieceLocation);
            // add capture move
            moveVector.emplace_back(
                    MoveBuilder(pieceType, fromSquare)
                        .setTarget(nearestPieceLocation)
                        .setCapture(capturePiece, nearestPieceLocation)
                        .build()
            );
            logger.debug(moveVector.back());
        }
    }

    for (const auto& toSquare : scope) {
        // add normal moves
        moveVector.emplace_back(
                MoveBuilder(pieceType, fromSquare)
                    .setTarget(toSquare)
                    .build()
        );
        logger.debug(moveVector.back());
    }

}