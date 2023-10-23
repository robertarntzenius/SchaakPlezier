#pragma once

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
    NoType = 0,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
};

enum Color : int {
    White = 1,
    Black = -1,
};

struct Piece {
    public:
        int offsets[8] = {0};
        int color;
        PieceType type;
        int numOffsets;
};

class PieceFactory {
    public:
        PieceFactory() = default;
        ~PieceFactory() = default;

        Piece * constructPiece(Color color, PieceType type) const {
            Piece* newPiece = new Piece;

            newPiece->color = color;
            newPiece->type = type;

            switch (type) {
                case Pawn:
                    newPiece->numOffsets = 4;

                    newPiece->offsets[0] = 2 * Offsets::North * color;
                    newPiece->offsets[1] = Offsets::NorthWest * color;
                    newPiece->offsets[2] = Offsets::North * color;
                    newPiece->offsets[3] = Offsets::NorthEast * color;
                    break;

                case Queen:
                    newPiece->numOffsets = 4;

                    newPiece->offsets[0] = Offsets::NorthWest;
                    newPiece->offsets[1] = Offsets::North;
                    newPiece->offsets[2] = Offsets::NorthEast;
                    newPiece->offsets[3] = Offsets::East;
                    break;

                case Rook:
                    newPiece->numOffsets = 2;

                    newPiece->offsets[0] = Offsets::North;
                    newPiece->offsets[1] = Offsets::East;
                    break;

                case Bishop:
                    newPiece->numOffsets = 2;

                    newPiece->offsets[0] = Offsets::NorthWest;
                    newPiece->offsets[1] = Offsets::NorthEast;
                    break;

                case Knight:
                    newPiece->numOffsets = 8;

                    newPiece->offsets[0] = Offsets::NorthWest + Offsets::North;
                    newPiece->offsets[1] = Offsets::NorthEast + Offsets::North;
                    newPiece->offsets[2] = Offsets::NorthWest + Offsets::West;
                    newPiece->offsets[3] = Offsets::NorthEast + Offsets::East;
                    newPiece->offsets[4] = Offsets::SouthWest + Offsets::West;
                    newPiece->offsets[5] = Offsets::SouthEast + Offsets::East;
                    newPiece->offsets[6] = Offsets::SouthWest + Offsets::South;
                    newPiece->offsets[7] = Offsets::SouthEast + Offsets::South;
                    break;

                case King:
                    newPiece->numOffsets = 8;

                    newPiece->offsets[0] = Offsets::NorthWest;
                    newPiece->offsets[1] = Offsets::North;
                    newPiece->offsets[2] = Offsets::NorthEast;
                    newPiece->offsets[3] = Offsets::West;
                    newPiece->offsets[4] = Offsets::East;
                    newPiece->offsets[5] = Offsets::SouthWest;
                    newPiece->offsets[6] = Offsets::South;
                    newPiece->offsets[7] = Offsets::SouthEast;
                    break;

                default:
                    delete newPiece;
                    return nullptr;
            }

            return newPiece;
        }
};