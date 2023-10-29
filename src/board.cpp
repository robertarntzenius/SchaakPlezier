#include "board.h"
#include "bitboard.h"
#include <iostream>

Board::Board()
    : wKC(true), wQC(true), bKC(true), bQC(true), enPassant(-1), 
    turn(Color::White), 
    whitePieces(InitPieces(Color::White)), 
    blackPieces(InitPieces(Color::Black)),
    white   ( Bitboard( ((1UL << 16) -1) << 48 ) ), 
    black   ( Bitboard( (1UL << 16) -1) ), 
    pawns   ( Bitboard( (((1UL << 8) -1) << 8) | (((1UL << 8) -1) << 40) ) ), 
    knights ( Bitboard( (1UL << 1) | (1UL << 6) | (1UL << 57) | (1UL << 62) ) ), 
    bishops ( Bitboard( (1UL << 2) | (1UL << 5) | (1UL << 58) | (1UL << 61)) ), 
    rooks   ( Bitboard( (1UL) | (1UL << 7) | (1UL << 56) | (1UL << 63)) ), 
    queens  ( Bitboard( (1UL << 3) | (1UL << 59) ) ), 
    kings   ( Bitboard( (1UL << 4) | (1UL << 60) ) )
{
    std::cout << white << std::endl;
    std::cout << black << std::endl;
}

Board::~Board() {
    for (Piece* piece : whitePieces)
        delete piece;

    for (Piece* piece : blackPieces)
        delete piece;
}

int *Board::getPossibleMoves()
{
    return nullptr;
}

std::vector<Piece*> Board::InitPieces(Color color) const
{
    PieceFactory factory;

    std::vector<Piece*> pieceVector;

    for (int i = 0; i < BOARDSIZE; i++) {
        pieceVector.emplace_back(factory.constructPiece(color, PieceType::Pawn));
    }

    pieceVector.emplace_back(factory.constructPiece(color, PieceType::Knight));
    pieceVector.emplace_back(factory.constructPiece(color, PieceType::Knight));
    pieceVector.emplace_back(factory.constructPiece(color, PieceType::Bishop));
    pieceVector.emplace_back(factory.constructPiece(color, PieceType::Bishop));
    pieceVector.emplace_back(factory.constructPiece(color, PieceType::Rook));
    pieceVector.emplace_back(factory.constructPiece(color, PieceType::Rook));
    pieceVector.emplace_back(factory.constructPiece(color, PieceType::Queen));
    pieceVector.emplace_back(factory.constructPiece(color, PieceType::King));

    return pieceVector;
}
