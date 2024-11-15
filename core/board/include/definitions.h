#pragma once

#include "types.h"
#include "bitboard.h"

#include <iostream>
#include <stdexcept>
#include <cassert>

#include <unordered_map>
#include <array>
#include <string>
#include <random>

constexpr const char *defaultStartingFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
constexpr const char *testFEN1 = "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2";
constexpr const char *testFEN2 = "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/8/PPPBBPPP/R3K2R b KQ - 3 2";
constexpr const char *testFEN3 = "rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9";
constexpr const char *testFEN4 = "rnbqkbnr/p1p1pppp/1p6/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3";
constexpr const char *gameOverFEN = "k7/2KP4/8/8/8/8/8/8 b - - 20 10";


#ifdef DEBUG

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

constexpr BoardState defaultBoardState = {
    White, false, false, false, false, NoSquare, 0, 0, 0
};

[[nodiscard]] static uint64_t randomKeyGenerator () {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<uint64_t> distribution;

    return distribution(gen);
};

[[nodiscard]] static std::array<std::array<std::array<uint64_t, NrPiecetypes>, NrColors>, NrSquares> initZobrist()
{
    std::array<std::array<std::array<uint64_t, NrPiecetypes>, NrColors>, NrSquares> table{};

    for (auto &square : table) {
        for (auto &color : square) {
            for (auto &value : color) {
                // Generate a random uint64_t
                value = randomKeyGenerator();
            }
        }
    }
    return table;
}


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

enum GameResult : uint8_t {
    NOT_OVER,
    WHITE_WIN_BY_CHECKMATE,
    BLACK_WIN_BY_CHECKMATE,
    WHITE_WIN_BY_TIME_OUT,
    BLACK_WIN_BY_TIME_OUT,
    WHITE_WIN_BY_FORFEIT,
    BLACK_WIN_BY_FORFEIT,
    DRAW_BY_STALEMATE,
    DRAW_BY_INSUFFICIENT_MATERIAL,
    DRAW_BY_REPETITION,
    DRAW_BY_50_MOVES,
};

const std::unordered_map<GameResult, std::string> gameResultStringMap = {
    {NOT_OVER, "Game is not over"},
    {WHITE_WIN_BY_CHECKMATE, "White won by checkmate!"},
    {BLACK_WIN_BY_CHECKMATE, "Black won by checkmate!"},
    {WHITE_WIN_BY_TIME_OUT, "White won by time out!"},
    {BLACK_WIN_BY_TIME_OUT, "Black won by time out!"},
    {WHITE_WIN_BY_FORFEIT, "White won by forfeit!"},
    {BLACK_WIN_BY_FORFEIT, "Black won by forfeit!"},
    {DRAW_BY_STALEMATE, "Draw by stalemate!"},
    {DRAW_BY_INSUFFICIENT_MATERIAL, "Draw by insufficient material!"},
    {DRAW_BY_REPETITION, "Draw by repetition!"},
    {DRAW_BY_50_MOVES, "Draw by 50 move rule!"},
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
    {Pawn,   "pawn"},
    {Knight, "knight"},
    {Bishop, "bishop"},
    {Rook,   "rook"},
    {Queen,  "queen"},
    {King,   "king"},
    {NoType, "NoType"}
};

const std::unordered_map<std::string, Piecetype> stringPiecetypeMap = {
    {"pawn", Pawn},
    {"knight", Knight},
    {"bishop", Bishop},
    {"rook", Rook},
    {"queen", Queen},
    {"king", King},
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

static std::ostream& operator<<(std::ostream &os, const GameResult &gameResult) {
    if (gameResult != NOT_OVER) {
        os << gameResultStringMap.at(gameResult);
    }
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
    }

    return os;
}

static std::ostream& operator<<(std::ostream &os, const BoardState &boardState) {
    os << "activePlayer: " << boardState.activePlayer << std::endl;
    os << "wKC: " << boardState.wKC << ", wQC: "<< boardState.wQC << ", bKC: "<< boardState.bKC << ", bQC: "<< boardState.bQC << std::endl;
    os << "halfMoveClock: " << boardState.halfMoveClock << "fullMoveNumber: " << boardState.fullMoveNumber << std::endl;
    os << "enPassant: " << boardState.enPassantSquare <<std::endl;
    os << "hash: " << boardState.hash <<std::endl;
    return os;
}
