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

constexpr Piece* Pawn(Color color) {
    Piece* piece = new Piece();
    piece->color = color;
    piece->type = PieceType::Pawn;
    piece->numOffsets = 4;
    
    piece->offsets[0] = 2 * Offsets::NorthWest * color;
    piece->offsets[1] = Offsets::NorthWest * color;
    piece->offsets[2] = Offsets::North * color;
    piece->offsets[3] = Offsets::NorthEast * color;

    return piece;
} 

constexpr Piece* Queen(Color color) {
    Piece* piece = new Piece();

    piece->color = color;
    piece->type = PieceType::Queen;
    piece->numOffsets = 4;

    piece->offsets[0] = Offsets::NorthWest;
    piece->offsets[1] = Offsets::North;
    piece->offsets[2] = Offsets::NorthEast;
    piece->offsets[3] = Offsets::East;
    
    return piece;
} 

constexpr Piece* Rook(Color color) {
    Piece* piece = new Piece();

    piece->color = color;
    piece->type = PieceType::Rook;
    piece->numOffsets = 2;

    piece->offsets[0] = Offsets::North;
    piece->offsets[1] = Offsets::East;
    
    return piece;
} 

constexpr Piece* Bishop(Color color) {
    Piece* piece = new Piece();

    piece->color = color;
    piece->type = PieceType::Bishop;
    piece->numOffsets = 2;

    piece->offsets[0] = Offsets::NorthWest;
    piece->offsets[1] = Offsets::NorthEast;
    
    return piece;
} 

constexpr Piece* King(Color color) {
    Piece* piece = new Piece();

    piece->color = color;
    piece->type = PieceType::Queen;
    piece->numOffsets = 8;

    piece->offsets[0] = Offsets::NorthWest;
    piece->offsets[1] = Offsets::North;
    piece->offsets[2] = Offsets::NorthEast;
    piece->offsets[3] = Offsets::West;
    piece->offsets[4] = Offsets::East;
    piece->offsets[5] = Offsets::SouthWest;
    piece->offsets[6] = Offsets::South;
    piece->offsets[7] = Offsets::SouthEast;

    return piece;
} 

constexpr Piece* Knight(Color color) {
    Piece* piece = new Piece();

    piece->color = color;
    piece->type = PieceType::Bishop;
    piece->numOffsets = 8;

    piece->offsets[0] = Offsets::NorthWest + Offsets::North;
    piece->offsets[1] = Offsets::NorthEast + Offsets::North;
    piece->offsets[2] = Offsets::NorthWest + Offsets::West;
    piece->offsets[3] = Offsets::NorthEast + Offsets::East;
    piece->offsets[4] = Offsets::SouthWest + Offsets::West;
    piece->offsets[5] = Offsets::SouthEast + Offsets::East;
    piece->offsets[6] = Offsets::SouthWest + Offsets::South;
    piece->offsets[7] = Offsets::SouthEast + Offsets::South;
    
    return piece;
}