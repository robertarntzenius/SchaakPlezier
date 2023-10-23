#pragma once
#include "piece.h"

#define BOARDSIZE 8

typedef unsigned long bitboard;

struct Move {
    int from;
    int to;
    PieceType pType;
};

class Board {
    public:
        // Board() : FENSTRING ;

        Board();
        ~Board() = default;
        int* getPossibleMoves();


    private:
        const Piece** InitPieces(Color color) const;

        bool wKC, wQC, bKC, bQC;
        int enPassant;

        Color turn;

        Piece* whitePieces[2*BOARDSIZE];
        Piece* blackPieces[2*BOARDSIZE];

        bitboard white, black, pawns, knights, bishops, rooks, queens, kings;

};