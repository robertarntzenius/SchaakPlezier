#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <algorithm>

#define BOARDSIZE 8
#define defaultStartingFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define testFEN1 "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2"
#define testFEN2 "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2"
#define testFEN3 "rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9"

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

enum Square : int {
    a8=0, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, noSquare
};

enum Offsets : int {
    North = -8,
    NorthEast = -7,
    East = 1,
    SouthEast = 9,
    South = 8,
    SouthWest = 7,
    West = -1,
    NorthWest = -9,
};

enum PieceType {
    wPawn = 0,
    bPawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    NoType
};

static const std::unordered_map<PieceType, std::string> pieceTypeStrings = {
    {PieceType::NoType, "NoType"},
    {PieceType::wPawn, "White Pawn"},
    {PieceType::bPawn, "Black Pawn"},
    {PieceType::Knight, "Knight"},
    {PieceType::Bishop, "Bishop"},
    {PieceType::Rook, "Rook"},
    {PieceType::Queen, "Queen"},
    {PieceType::King, "King"},
};

static const std::unordered_map<Square, std::string> squareStrings = {
        {a8, "a8"}, {b8, "b8"}, {c8, "c8"}, {d8, "d8"}, {e8, "e8"}, {f8, "f8"}, {g8, "g8"}, {h8, "h8"},
        {a7, "a7"}, {b7, "b7"}, {c7, "c7"}, {d7, "d7"}, {e7, "e7"}, {f7, "f7"}, {g7, "g7"}, {h7, "h7"},
        {a6, "a6"}, {b6, "b6"}, {c6, "c6"}, {d6, "d6"}, {e6, "e6"}, {f6, "f6"}, {g6, "g6"}, {h6, "h6"},
        {a5, "a5"}, {b5, "b5"}, {c5, "c5"}, {d5, "d5"}, {e5, "e5"}, {f5, "f5"}, {g5, "g5"}, {h5, "h5"},
        {a4, "a4"}, {b4, "b4"}, {c4, "c4"}, {d4, "d4"}, {e4, "e4"}, {f4, "f4"}, {g4, "g4"}, {h4, "h4"},
        {a3, "a3"}, {b3, "b3"}, {c3, "c3"}, {d3, "d3"}, {e3, "e3"}, {f3, "f3"}, {g3, "g3"}, {h3, "h3"},
        {a2, "a2"}, {b2, "b2"}, {c2, "c2"}, {d2, "d2"}, {e2, "e2"}, {f2, "f2"}, {g2, "g2"}, {h2, "h2"},
        {a1, "a1"}, {b1, "b1"}, {c1, "c1"}, {d1, "d1"}, {e1, "e1"}, {f1, "f1"}, {g1, "g1"}, {h1, "h1"},
        {noSquare, "noSquare"}
};

Square squareFromString(const std::string& algebraic);

enum Color : int {
    White = 1,
    Black = -1,
};

constexpr Color invertColor (Color color) {
    return (color == Color::White) ? Color::Black : Color::White;
}

struct Piece
{
    PieceType type = PieceType::NoType;
    Square square;

    Piece(PieceType type, Square square)
        : type(type),
          square(square) {}

    friend std::ostream& operator<<(std::ostream& os, const Piece& piece)
    {
        os  << '[' << pieceTypeStrings.at(piece.type)
            << ':' << squareStrings.at(piece.square) << ']';
        return os;
    }
};

[[nodiscard]] constexpr Square intToSquare(int i) {
    return (a8 <= i && i <= h1) ? static_cast<Square>(i) : throw std::out_of_range("Invalid chessboard square");
}


struct Move {
    Piece piece;
    Square target;
    PieceType capture; // TODO FIXME change to piece

    Move(Square from, Square target, PieceType pType, PieceType capture)
            : piece(pType, from),
              target(target),
              capture(capture) {}

    friend std::ostream& operator<<(std::ostream& os, const Move& move) {
        std::string fromSquare = static_cast<char>('a' + static_cast<int>(move.piece.square) % 8) +
                                 std::to_string(8 - static_cast<int>(move.piece.square) / 8);
        std::string toSquare = static_cast<char>('a' + static_cast<int>(move.target) % 8) +
                               std::to_string(8 - static_cast<int>(move.target) / 8);

        const std::string& pTypeStr = pieceTypeStrings.at(move.piece.type);;
        const std::string& captureStr = pieceTypeStrings.at(move.capture);;
        os << "From: " << fromSquare << ", To: " << toSquare << ", Piece: " << pTypeStr << ", Capture: " << captureStr;
        return os;
    }
};

const char* getPieceChar(PieceType pType, Color color);

//
//class PieceFactory {
//    public:
//        PieceFactory() = default;
//        ~PieceFactory() = default;
//
//        Piece * constructPiece(PieceType type) const {
//            Piece* newPiece = new Piece;
//
//            newPiece->type = type;
//
//            switch (type) {
//                case wPawn:
//                    newPiece->numOffsets = 4;
//
//                    newPiece->offsets[0] = 2 * Offsets::North;
//                    newPiece->offsets[1] = Offsets::NorthWest;
//                    newPiece->offsets[2] = Offsets::North;
//                    newPiece->offsets[3] = Offsets::NorthEast;
//                    break;
//            case bPawn:
//                    newPiece->numOffsets = 4;
//
//                    newPiece->offsets[0] = -2 * Offsets::North;
//                    newPiece->offsets[1] = -Offsets::NorthWest;
//                    newPiece->offsets[2] = -Offsets::North;
//                    newPiece->offsets[3] = -Offsets::NorthEast;
//                    break;
//                case Queen:
//                    newPiece->numOffsets = 4;
//
//                    newPiece->offsets[0] = Offsets::NorthWest;
//                    newPiece->offsets[1] = Offsets::North;
//                    newPiece->offsets[2] = Offsets::NorthEast;
//                    newPiece->offsets[3] = Offsets::East;
//                    break;
//
//                case Rook:
//                    newPiece->numOffsets = 2;
//
//                    newPiece->offsets[0] = Offsets::North;
//                    newPiece->offsets[1] = Offsets::East;
//                    break;
//
//                case Bishop:
//                    newPiece->numOffsets = 2;
//
//                    newPiece->offsets[0] = Offsets::NorthWest;
//                    newPiece->offsets[1] = Offsets::NorthEast;
//                    break;
//
//                case Knight:
//                    newPiece->numOffsets = 8;
//
//                    newPiece->offsets[0] = Offsets::NorthWest + Offsets::North;
//                    newPiece->offsets[1] = Offsets::NorthEast + Offsets::North;
//                    newPiece->offsets[2] = Offsets::NorthWest + Offsets::West;
//                    newPiece->offsets[3] = Offsets::NorthEast + Offsets::East;
//                    newPiece->offsets[4] = Offsets::SouthWest + Offsets::West;
//                    newPiece->offsets[5] = Offsets::SouthEast + Offsets::East;
//                    newPiece->offsets[6] = Offsets::SouthWest + Offsets::South;
//                    newPiece->offsets[7] = Offsets::SouthEast + Offsets::South;
//                    break;
//
//                case King:
//                    newPiece->numOffsets = 8;
//
//                    newPiece->offsets[0] = Offsets::NorthWest;
//                    newPiece->offsets[1] = Offsets::North;
//                    newPiece->offsets[2] = Offsets::NorthEast;
//                    newPiece->offsets[3] = Offsets::West;
//                    newPiece->offsets[4] = Offsets::East;
//                    newPiece->offsets[5] = Offsets::SouthWest;
//                    newPiece->offsets[6] = Offsets::South;
//                    newPiece->offsets[7] = Offsets::SouthEast;
//                    break;
//
//                default:
//                    delete newPiece;
//                    return nullptr;
//            }
//
//            return newPiece;
//        }
//};