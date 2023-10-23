#pragma once

#include <vector>

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
        // Board() : FENSTRING ; TODO

        Board();
        ~Board();
        int* getPossibleMoves();


        // FIXME operator overload ostream
        void show() const;


    private:
        std::vector<Piece*> InitPieces(Color color) const;

        bool wKC, wQC, bKC, bQC;
        int enPassant;

        Color turn;


        // FIXME aparte piece structuur zo (moet gewoon 1 van elke type)
        std::vector<Piece*> whitePieces;
        std::vector<Piece*> blackPieces;

        bitboard white, black, pawns, knights, bishops, rooks, queens, kings;

};