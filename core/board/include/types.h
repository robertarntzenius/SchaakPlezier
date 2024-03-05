#pragma once

#include <cstdint>

constexpr int BOARD_DIMENSIONS = 8;
constexpr int BOARD_SIZE = BOARD_DIMENSIONS * BOARD_DIMENSIONS;


enum Color : uint8_t {
    White,
    Black,
    NrColors = 2
};

enum Square : int8_t {
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

enum Piecetype : uint8_t {
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    NrPiecetypes = 6,
    NrPromotiontypes = 4,
    NoType = 7
};

enum File : uint8_t {
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

enum Rank : uint8_t {
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

// NOTE: do not use for offset values, use the DirectionalOffset enum instead!
enum Direction : uint8_t {
    // Vertical directions
    North,
    South,

    // Horizontal directions
    West,
    East,

    // Diagonal directions
    NorthEast,
    NorthWest,
    SouthEast,
    SouthWest,

    FirstOrthogonal = North,
    LastOrthogonal = East,
    FirstDiagonal = NorthEast,
    LastDiagonal = SouthWest,
    NrDirections = 8
};

enum CastlingSide : uint8_t {
    wKingside,
    wQueenside,
    bKingside,
    bQueenside,
    NrCastlingRights = 4
};

struct Move {
    // General move info (must be set)
    Piecetype playerPiece = NoType;
    Square fromSquare   = NoSquare;
    Square targetSquare = NoSquare;

    // Move type flags
    bool isCastling     = false;
    bool isCapture      = false;
    bool isPromotion    = false;

    // Support move info (may have default values)
    Piecetype promotionPiece  = NoType;   // Promotions
    Piecetype capturePiece    = NoType;   // Captures
    Square captureSquare    = NoSquare;   // Captures
    Square newEnPassant     = NoSquare;   // Double pawn moves
 
    bool operator<=>(const Move&) const = default;
};

struct BoardState {
    Color activePlayer; //1
    bool wKC, wQC, bKC, bQC; //4 
    Square enPassantSquare; //1
    uint16_t fullMoveNumber, halfMoveClock; // 4
};
