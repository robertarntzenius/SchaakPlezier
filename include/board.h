#pragma once
#include "piece.h"

#define BOARDSIZE 8

typedef bool bitboard[BOARDSIZE][BOARDSIZE];

struct Move {
    int from;
    int to;
    PieceType pType;

};

class Board {
    public:
        Board() = default;
        ~Board() = default;
        
        int* getPossibleMoves();


    private:

        bool wKC, wQC, bKC, bQC;
        int enPassant;

        Color turn;

        Piece* whitePieces[2*BOARDSIZE];
        Piece* blackPieces[2*BOARDSIZE];

        bitboard white, black, pawns, knights, bishops, rooks, queens, kings;

};