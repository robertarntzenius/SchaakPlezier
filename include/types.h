#pragma once

constexpr int BOARD_DIMENSIONS = 8;
constexpr int BOARD_SIZE = BOARD_DIMENSIONS * BOARD_DIMENSIONS;


enum Color : int {
    White,
    Black,
    NrColors = 2
};

enum Square : int {
    a8 = 0, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
    NrSquares = 64, NoSquare
};

enum Piecetype : int {
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    NrPiecetypes = 6,
    NrPromotiontypes = 4,
    NoType
};

enum File : int {
    FileA = 0,
    FileB,
    FileC,
    FileD,
    FileE,
    FileF,
    FileG,
    FileH,
    NrFiles = 8
};

enum Rank : int {
    Rank8 = 0,
    Rank7,
    Rank6,
    Rank5,
    Rank4,
    Rank3,
    Rank2,
    Rank1,
    NrRanks = 8
};

struct Move {
    const Square from   = NoSquare;
    const Square target = NoSquare;
    const Square extra  = NoSquare;

    const Piecetype playerPiece   = NoType;
    const Piecetype capturePiece  = NoType;
    const Piecetype promotionType = NoType;

    const enum {
        Basic,
        Capture,
        Promotion,
        PromotionCapture,
        EnPassantCapure,
        DoublePawn,
        Castling
    } type = Basic;
};



