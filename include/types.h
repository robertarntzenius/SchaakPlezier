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

    const enum {
        Basic,
        Capture,
        DoublePawn,
        Castle,
        Promotion
    } type;
};

struct CaptureMove : public Move {
    const Square captureSquare;
};

struct DoublePawnMove : public Move {
    const Square enPassantSquare;
};

struct CastleMove : public Move {
    const Move castleRookMove;
};

struct PromotionMove : public Move {
    const Piecetype promotionType;
};