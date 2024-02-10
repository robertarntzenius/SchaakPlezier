#pragma once

#include "types.h"

class MoveBuilder {
public:
    MoveBuilder(Piecetype playerPiece, Square fromSquare)
            : move{playerPiece, fromSquare} {}

    auto setTarget(Square targetSquare) {
        move.targetSquare = targetSquare;
        return *this;
    }

    auto setCapture(Piecetype capturePiece, Square captureSquare) {
        move.capturePiece = capturePiece;
        move.captureSquare = captureSquare;
        move.isCapture = true;
        return *this;
    }

    auto setPromotion(Piecetype promotionPiece) {
        move.promotionPiece = promotionPiece;
        move.isPromotion = true;
        return *this;
    }

    auto setEnPassant(Square newEnPassant) {
        move.newEnPassant = newEnPassant;
        return *this;
    }

    auto setCastling() {
        move.isCastling = true;
        return *this;
    }


    [[nodiscard]] Move build() && {
        return move;
    }

private:
    Move move;
};