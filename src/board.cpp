#include "board.h"

Board::Board() 
    : wKC(true), wQC(true), bKC(true), bQC(true), enPassant(-1), 
    turn(Color::White), 
    whitePieces(InitPieces(Color::White)), 
    blackPieces(),
    white   ( ((1UL << 16) -1) << 48 ), 
    black   ( (1UL << 16) -1), 
    pawns   ( (((1UL << 8) -1) << 8) | (((1UL << 8) -1) << 40) ), 
    knights ( (1UL << 1) | (1UL << 6) | (1UL << 57) | (1UL << 62) ), 
    bishops ( (1UL << 2) | (1UL << 5) | (1UL << 58) | (1UL << 61)), 
    rooks   ( (1UL) | (1UL << 7) | (1UL << 56) | (1UL << 63)), 
    queens  ( (1UL << 3) | (1UL << 59) ), 
    kings   ( (1UL << 4) | (1UL << 60) )
{
    
}

int *Board::getPossibleMoves()
{
    return nullptr;
}

const Piece** Board::InitPieces(Color color) const
{
    Piece** pieceList = new Piece*[2*BOARDSIZE];

    for (int i=0; i<BOARDSIZE; i++) {
        pieceList[i] = Pawn(color);
    }

    pieceList[8] = Knight(color);
    pieceList[9] = Knight(color);
    pieceList[10] = Bishop(color);
    pieceList[11] = Bishop(color);
    pieceList[12] = Rook(color);
    pieceList[13] = Rook(color);
    pieceList[14] = Queen(color);
    pieceList[15] = King(color);

    return pieceList;
}
