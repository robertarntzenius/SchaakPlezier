#pragma once

constexpr int BOARD_DIMENSIONS = 8;
constexpr int BOARD_SIZE = BOARD_DIMENSIONS * BOARD_DIMENSIONS;


enum Color : int {
    White,
    Black,
    NrColors = 2
};

enum Square : int {
    a8=0, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
    NrSquares = 64, NoSquare
};

// NOTE: maybe useful ?? IDK

//enum File : int {
//    A = 0, B, C, D, E, F, G, H, NrFiles = 8
//};

//enum Rank : int {
//    Rank1 = 7,
//    Rank2 = 6,
//    Rank3 = 5,
//    Rank4 = 4,
//    Rank5 = 3,
//    Rank6 = 2,
//    Rank7 = 1,
//    Rank8 = 0
//};

enum Piecetype : int {
    NoType = 0,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
};

struct Move {
    // Basic information needed for all moves
    const Square from;
    const Square target;

//    constexpr Move(Square from, Square target)
//        : from(from),
//          target(target)
//    {}
};

struct CaptureMove : public Move {
    const Square captureSquare;

//    constexpr CaptureMove(Square from, Square target, Square captureSquare)
//        : Move(from, target),
//          captureSquare(captureSquare)
//    {}
};

struct DoublePawnMove : public Move {
    const Square enPassantSquare;

//    constexpr DoublePawnMove(Square from, Square target, Square enPassantSquare)
//        : Move(from, target),
//          enPassantSquare(enPassantSquare)
//    {}
};

struct CastleMove : public Move {
    const Move castleRookMove;

//    constexpr CastleMove(Square from, Square target, Square rookFrom, Square rookTarget)
//        : Move(from, target),
//          castleRookMove(from, target)
//    {}
};

struct PromotionMove : public Move {
    const Piecetype promotionType;


//    constexpr PromotionMove(Square from, Square target, Piecetype promotionType)
//        : Move(from, target),
//          promotionType(promotionType)
//    {}
};