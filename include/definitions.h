#pragma once

#include "types.h"
#include "bitboard.h"

#include <iostream>
#include <stdexcept>

#include <unordered_map>
#include <array>
#include <string>

constexpr const char *defaultStartingFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

#ifdef DEBUG
// _assert will be compiled in Debug
#define _assert(expr) \
    if (!(expr)) { \
        std::string logstream = "-- [ASSERTION FAILED] at " + \
            std::string(__FILE__) + ":" + std::to_string(__LINE__); \
        std::cerr << logstream << std::endl; \
        std::cerr << "See ../../build/Debug/Schaakplezier.log for more information" << std::endl; \
        exit(-1); \
    }
#else
// _assert will not be compiled in Release
#define _assert(expr) ((void)0)
#endif

[[nodiscard]] constexpr Square intToSquare(int i) {
    if (i < a8 || i > h1) {
        return NoSquare;
    }

    return static_cast<Square>(i);
}

// Conversions and Comparisons
[[nodiscard]] constexpr Color operator~(Color color) {
    return static_cast<Color>((color + 1) % NrColors);
}

[[nodiscard]] constexpr Rank squareToRank(Square square) {
    return static_cast<Rank>(square / BOARD_DIMENSIONS);
}

[[nodiscard]] constexpr File squareToFile(Square square) {
    return static_cast<File>(square % BOARD_DIMENSIONS);
}

[[nodiscard]] constexpr Square rankFileToSquare(Rank rank, File file) {
    return static_cast<Square>(rank*BOARD_DIMENSIONS + file);
}

constexpr std::array<Piecetype, NrPromotiontypes> promotionPiecetypes {
    Knight, Bishop, Rook, Queen
};

/**
 * Directional offsets based on L-shifts
 */
enum DirectionalOffset : int8_t {
    OFFSET_NORTH =  8,
    OFFSET_SOUTH = -8,
    OFFSET_WEST  =  1,
    OFFSET_EAST  = -1,
    OFFSET_NORTHWEST = OFFSET_NORTH + OFFSET_WEST,
    OFFSET_NORTHEAST = OFFSET_NORTH + OFFSET_EAST,
    OFFSET_SOUTHWEST = OFFSET_SOUTH + OFFSET_WEST,
    OFFSET_SOUTHEAST = OFFSET_SOUTH + OFFSET_EAST,

    NO_OFFSET = 0
};

// Stringmaps for logging
const std::unordered_map<Square, std::string> squareStringMap = {
    {a8, "a8"}, {b8, "b8"}, {c8, "c8"}, {d8, "d8"}, {e8, "e8"}, {f8, "f8"}, {g8, "g8"}, {h8, "h8"},
    {a7, "a7"}, {b7, "b7"}, {c7, "c7"}, {d7, "d7"}, {e7, "e7"}, {f7, "f7"}, {g7, "g7"}, {h7, "h7"},
    {a6, "a6"}, {b6, "b6"}, {c6, "c6"}, {d6, "d6"}, {e6, "e6"}, {f6, "f6"}, {g6, "g6"}, {h6, "h6"},
    {a5, "a5"}, {b5, "b5"}, {c5, "c5"}, {d5, "d5"}, {e5, "e5"}, {f5, "f5"}, {g5, "g5"}, {h5, "h5"},
    {a4, "a4"}, {b4, "b4"}, {c4, "c4"}, {d4, "d4"}, {e4, "e4"}, {f4, "f4"}, {g4, "g4"}, {h4, "h4"},
    {a3, "a3"}, {b3, "b3"}, {c3, "c3"}, {d3, "d3"}, {e3, "e3"}, {f3, "f3"}, {g3, "g3"}, {h3, "h3"},
    {a2, "a2"}, {b2, "b2"}, {c2, "c2"}, {d2, "d2"}, {e2, "e2"}, {f2, "f2"}, {g2, "g2"}, {h2, "h2"},
    {a1, "a1"}, {b1, "b1"}, {c1, "c1"}, {d1, "d1"}, {e1, "e1"}, {f1, "f1"}, {g1, "g1"}, {h1, "h1"},
    {NoSquare, "NoSquare"}, {NrSquares, "NrSquares"},
};

const std::unordered_map<std::string, Square> stringSquareMap = {
    {"-", NoSquare},
    {"a8", a8}, {"b8", b8}, {"c8", c8}, {"d8", d8}, {"e8", e8}, {"f8", f8}, {"g8", g8}, {"h8", h8},
    {"a7", a7}, {"b7", b7}, {"c7", c7}, {"d7", d7}, {"e7", e7}, {"f7", f7}, {"g7", g7}, {"h7", h7},
    {"a6", a6}, {"b6", b6}, {"c6", c6}, {"d6", d6}, {"e6", e6}, {"f6", f6}, {"g6", g6}, {"h6", h6},
    {"a5", a5}, {"b5", b5}, {"c5", c5}, {"d5", d5}, {"e5", e5}, {"f5", f5}, {"g5", g5}, {"h5", h5},
    {"a4", a4}, {"b4", b4}, {"c4", c4}, {"d4", d4}, {"e4", e4}, {"f4", f4}, {"g4", g4}, {"h4", h4},
    {"a3", a3}, {"b3", b3}, {"c3", c3}, {"d3", d3}, {"e3", e3}, {"f3", f3}, {"g3", g3}, {"h3", h3},
    {"a2", a2}, {"b2", b2}, {"c2", c2}, {"d2", d2}, {"e2", e2}, {"f2", f2}, {"g2", g2}, {"h2", h2},
    {"a1", a1}, {"b1", b1}, {"c1", c1}, {"d1", d1}, {"e1", e1}, {"f1", f1}, {"g1", g1}, {"h1", h1},
};

const std::unordered_map<File, std::string> fileStringMap = {
       {FileA, "A"},
       {FileB, "B"},
       {FileC, "C"},
       {FileD, "D"},
       {FileE, "E"},
       {FileF, "F"},
       {FileG, "G"},
       {FileH, "H"}
};

const std::unordered_map<Rank, std::string> rankStringMap = {
       {Rank1, "1"},
       {Rank2, "2"},
       {Rank3, "3"},
       {Rank4, "4"},
       {Rank5, "5"},
       {Rank6, "6"},
       {Rank7, "7"},
       {Rank8, "8"}
};

const std::unordered_map<char, Color> charColorMap = {
    {'w', White},
    {'b', Black},
};

const std::unordered_map<Color, std::string> colorStringMap = {
    {White, "white"},
    {Black, "black"},
    {NrColors, "NrColors"},
};

const std::unordered_map<char, Piecetype> charPiecetypeMap = {
    {'p', Pawn},    {'P', Pawn},
    {'n', Knight},  {'N', Knight},
    {'b', Bishop},  {'B', Bishop},
    {'r', Rook},    {'R', Rook},
    {'q', Queen},   {'Q', Queen},
    {'k', King},    {'K', King},
};

const std::unordered_map<Piecetype, char> whitePiecetypeCharMap = {
    {Pawn, 'P'},
    {Knight, 'N'},
    {Bishop, 'B'},
    {Rook, 'R'},
    {Queen, 'Q'},
    {King, 'K'}
};

const std::unordered_map<Piecetype, char> blackPiecetypeCharMap = {
    {Pawn,   'p'},
    {Knight, 'n'},
    {Bishop, 'b'},
    {Rook,   'r'},
    {Queen,  'q'},
    {King,   'k'},
};

const std::unordered_map<Piecetype, std::string> piecetypeStringMap = {
    {NoType, "NoType"},
    {Pawn,   "pawn"},
    {Knight, "knight"},
    {Bishop, "bishop"},
    {Rook,   "rook"},
    {Queen,  "queen"},
    {King,   "king"},
};

const std::unordered_map<Direction, std::string> directionStringMap = {
       {North, "North"},
       {South, "South"},
       {East, "East"},
       {West, "West"},
       {NorthEast, "NorthEast"},
       {NorthWest, "NorthWest"},
       {SouthEast, "SouthEast"},
       {SouthWest, "SouthWest"}
};

// Operator overloads for logging
static std::ostream& operator<<(std::ostream &os, const Square &square) {
    os << squareStringMap.at(square);
    return os;
}

static std::ostream& operator<<(std::ostream &os, const Color &color) {
    os << colorStringMap.at(color);
    return os;
}

static std::ostream& operator<<(std::ostream &os, const Rank &rank) {
    os << rankStringMap.at(rank);
    return os;
}

static std::ostream& operator<<(std::ostream &os, const File &file) {
    os << fileStringMap.at(file);
    return os;
}

static std::ostream& operator<<(std::ostream &os, const Piecetype &piecetype) {
    os << piecetypeStringMap.at(piecetype);
    return os;
}

static std::ostream& operator<<(std::ostream &os, const Bitboard &bitboard) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        os << ((bitboard.test(i)) ? '1' : '0');

        if (i % BOARD_DIMENSIONS == BOARD_DIMENSIONS - 1) {
            os << '\n';
        }
    }
    return os;
}

static std::ostream& operator<<(std::ostream &os, const Direction &direction) {
    os << directionStringMap.at(direction);
    return os;
}

static std::ostream& operator<<(std::ostream &os, const Move &move) {
    os << "[" << move.playerPiece << " - " << move.fromSquare << move.targetSquare << "]";

    if (move.newEnPassant != NoSquare) {
        os << " | DoublePawn | "
           << "newEnPassantSquare:" << move.newEnPassant;
    }

    if (move.isCapture) {
        os << " | Capture | "
           << "capturePiece:" << move.capturePiece
           << ", captureSquare:" << move.captureSquare;
    }

    if (move.isPromotion) {
        os << " | Promotion | "
           << "promotionType:" << move.promotionPiece;
    }

    if (move.isCastling) {
        os << " | Castling | ";
        // os << ...;
    }

    return os;
}