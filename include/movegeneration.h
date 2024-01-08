#pragma once

#include "types.h"

namespace MoveGeneration {
    [[nodiscard]] static constexpr Move createMove(Square from, Square target, Piecetype playerPiece) {
        return {from, target, NoSquare, playerPiece, NoType, NoType, Move::Basic};
    }

    [[nodiscard]] static constexpr Move createCapture(Square from,
                                                      Square target,
                                                      Piecetype playerPiece,
                                                      Piecetype capturePiece) {
        return {from, target, NoSquare, playerPiece, capturePiece, NoType, Move::Capture};
    }

    [[nodiscard]] static constexpr Move createPromotionMove(Square from,
                                                            Square target,
                                                            Piecetype promotionType) {
        return {from, target, NoSquare, Pawn, NoType, promotionType, Move::Promotion};
    }

    [[nodiscard]] static constexpr Move createPromotionCapture(Square from,
                                                               Square target,
                                                               Piecetype capturePiece,
                                                               Piecetype promotionType) {
        return {from, target, NoSquare, Pawn, capturePiece, promotionType, Move::PromotionCapture};
    }

    [[nodiscard]] static constexpr Move createEnPassantCapture(Square from,
                                                               Square target,
                                                               Square capture,
                                                               Piecetype capturePiece) {
        return {from, target, capture, Pawn, capturePiece, NoType, Move::EnPassantCapture};
    }

    [[nodiscard]] static constexpr Move createDoublePawnMove(Square from,
                                                             Square target,
                                                             Square newEnPassant) {
        return {from, target, newEnPassant, Pawn, NoType, NoType, Move::DoublePawn};
    }

    [[nodiscard]] static constexpr Move createCastlingMove(Square from, Square target) {
        return {from, target, NoSquare, NoType, NoType, NoType, Move::Castling};
    }
}