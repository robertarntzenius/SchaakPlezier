#pragma once

enum PieceType {
    noType = 0,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

enum Color {
    white = 0,
    black
};

class Piece {
    public:
        Piece() = default;
        ~Piece() = default;
        virtual const int* getOffsets() const;


    private:
        const int _offsets;
        const int _color;
        PieceType _type;
};