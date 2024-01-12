#pragma once

#include "types.h"

class MoveBuilder {
public:
    [[nodiscard]] MoveBuilder(Piecetype playerPiece, Square fromSquare, Square targetSquare)
            : move{playerPiece, fromSquare, targetSquare} {}

    [[nodiscard]] MoveBuilder& setCapture(Piecetype capturePiece, Square captureSquare) {
        move.capturePiece = capturePiece;
        move.captureSquare = captureSquare;
        move.isCapture = true;
        return *this;
    }

    [[nodiscard]] MoveBuilder& setPromotion(Piecetype promotionPiece) {
        move.promotionPiece = promotionPiece;
        move.isPromotion = true;
        return *this;
    }

    [[nodiscard]] MoveBuilder& setEnPassant(Square newEnPassant) {
        move.newEnPassant = newEnPassant;
        return *this;
    }

    [[nodiscard]] MoveBuilder& setCastling() {
        move.isCastling = true;
        return *this;
    }


    [[nodiscard]] Move build() const {
        return move;
    }

private:
    Move move;
};



//namespace MoveGeneration {
//    [[nodiscard]] constexpr Move createMove(Square from, Square target, Piecetype playerPiece) {
//        return {from, target, NoSquare, playerPiece, NoType, NoType, Move::Basic};
//    }
//
//    [[nodiscard]] constexpr Move createCapture(Square from,
//                                               Square target,
//                                               Piecetype playerPiece,
//                                               Piecetype capturePiece) {
//        return {from, target, NoSquare, playerPiece, capturePiece, NoType, Move::Capture};
//    }
//
//    [[nodiscard]] constexpr Move createPromotionMove(Square from,
//                                                     Square target,
//                                                     Piecetype promotionType) {
//        return {from, target, NoSquare, Pawn, NoType, promotionType, Move::Promotion};
//    }
//
//    [[nodiscard]] constexpr Move createPromotionCapture(Square from,
//                                                        Square target,
//                                                        Piecetype capturePiece,
//                                                        Piecetype promotionType) {
//        return {from, target, NoSquare, Pawn, capturePiece, promotionType, Move::PromotionCapture};
//    }
//
//    [[nodiscard]] constexpr Move createEnPassantCapture(Square from,
//                                                        Square target,
//                                                        Square capture,
//                                                        Piecetype capturePiece) {
//        return {from, target, capture, Pawn, capturePiece, NoType, Move::EnPassantCapture};
//    }
//
//    [[nodiscard]] constexpr Move createDoublePawnMove(Square from,
//                                                      Square target,
//                                                      Square newEnPassant) {
//        return {from, target, newEnPassant, Pawn, NoType, NoType, Move::DoublePawn};
//    }
//
//    [[nodiscard]] constexpr Move createCastlingMove(Square from, Square target) {
//        return {from, target, NoSquare, NoType, NoType, NoType, Move::Castling};
//    }
//}