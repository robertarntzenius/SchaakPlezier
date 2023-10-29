#include <iostream>
#include <string>

#include "board.h"
#include "bitboard.h"

Board::Board()
    : wKC(true), wQC(true), bKC(true), bQC(true), enPassant(-1), halfMoves(0), fullMoves(0),
    turn    ( Color::White), 
    pieces  ( InitPieces()),
    white   ( Bitboard( ((1UL << 16) -1) << 48 ) ), 
    black   ( Bitboard( (1UL << 16) -1) ),
    pawns   ( Bitboard( (((1UL << 8) -1) << 8) | (((1UL << 8) -1) << 40) ) ), 
    knights ( Bitboard( (1UL << 1) | (1UL << 6) | (1UL << 57) | (1UL << 62) ) ), 
    bishops ( Bitboard( (1UL << 2) | (1UL << 5) | (1UL << 58) | (1UL << 61)) ), 
    rooks   ( Bitboard( (1UL) | (1UL << 7) | (1UL << 56) | (1UL << 63)) ), 
    queens  ( Bitboard( (1UL << 3) | (1UL << 59) ) ),
    kings   ( Bitboard( (1UL << 4) | (1UL << 60) ) )
{
    std::cout << "white" << std::endl;
    std::cout << white << std::endl;

    std::cout << "black" << std::endl;
    std::cout << black << std::endl;
}

Board::Board(const std::string& fenString)
    : wKC(true), wQC(true), bKC(true), bQC(true), enPassant(-1), halfMoves(0), fullMoves(0),
    turn(Color::White),
    pieces(InitPieces()),
    white   ( Bitboard( 0UL )),
    black   ( Bitboard( 0UL )),
    pawns   ( Bitboard( 0UL )),
    knights ( Bitboard( 0UL )),
    bishops ( Bitboard( 0UL )),
    rooks   ( Bitboard( 0UL )),
    queens  ( Bitboard( 0UL )),
    kings   ( Bitboard( 0UL ))

{
    InitializeBitboardsFromFEN(fenString);

    std::cout << "white" << std::endl;
    std::cout << white << std::endl;

    std::cout << "black" << std::endl;
    std::cout << black << std::endl;

    std::cout << "pawns" << std::endl;
    std::cout << pawns << std::endl;

    std::cout << "knights" << std::endl;
    std::cout << knights << std::endl;

    std::cout << "bishops" << std::endl;
    std::cout << bishops << std::endl;
    
    std::cout << "rooks" << std::endl;
    std::cout << rooks << std::endl;

    std::cout << "queens" << std::endl;
    std::cout << queens << std::endl;

    std::cout << "kings" << std::endl;
    std::cout << kings << std::endl;
}

void Board::InitializeBitboardsFromFEN(const std::string& fenString) {
    int square = 0;
    int FENIndex = 0;

    while ((square < 64) && (FENIndex < fenString.length())) {
        char ch = fenString[FENIndex];
        if (isalpha(ch)) {
            switch(ch) {
                case 'r':
                case 'R':
                    rooks.set(square);
                break;

                case 'n':
                case 'N':
                    knights.set(square);
                break;
                case 'b':
                case 'B':
                    bishops.set(square);
                break;
                case 'q':
                case 'Q':
                    queens.set(square);
                break;
                case 'k':
                case 'K':
                    kings.set(square);
                break;

                case 'p':
                case 'P':
                    pawns.set(square);
                break;

            default:
                std::cerr << "Robert fucked up" << std::endl;
            }
            if(isupper(ch)) {
                white.set(square);
            }
            else {
                black.set(square);
            }
            square++;
        }

        if (isdigit(ch)) {
            square += ch - '0';
        }
        
        FENIndex++;
    }

    turn = (fenString[++FENIndex] == 'w') ? Color::White : Color::Black;
    FENIndex += 2;
    wKC = (fenString[FENIndex++] != '-');
    wQC = (fenString[FENIndex++] != '-');
    bKC = (fenString[FENIndex++] != '-');
    bQC = (fenString[FENIndex++] != '-');

    enPassant = (fenString[++FENIndex] != '-') ? -1 : (fenString[FENIndex] - '0');
    FENIndex++;
    while ((fenString[++FENIndex] != ' ') && (FENIndex < fenString.length())) {
        if (!isdigit(fenString[FENIndex])) {
            std::cerr << "invalid FENstring" << std::endl;
            // FIXME implement real error handling
        }
        halfMoves *= 10;
        halfMoves += fenString[FENIndex] - '0';
    }

    while ((fenString[++FENIndex] != ' ') && (FENIndex < fenString.length())) {
        if (!isdigit(fenString[FENIndex])) {
            std::cerr << "invalid FENstring" << std::endl; 
            // FIXME implement real error handling
        }
        fullMoves *= 10;
        fullMoves += fenString[FENIndex] - '0';
    }
}

Board::~Board() {
    for (Piece* piece : pieces)
        delete piece;
}

int *Board::getPossibleMoves()
{
    return nullptr;
}

std::vector<Piece*> Board::InitPieces() const
{
    PieceFactory factory;

    std::vector<Piece*> pieceVector;

    pieceVector.emplace_back(factory.constructPiece(PieceType::wPawn));
    pieceVector.emplace_back(factory.constructPiece(PieceType::bPawn));
    pieceVector.emplace_back(factory.constructPiece(PieceType::Knight));
    pieceVector.emplace_back(factory.constructPiece(PieceType::Bishop));
    pieceVector.emplace_back(factory.constructPiece(PieceType::Rook));
    pieceVector.emplace_back(factory.constructPiece(PieceType::Queen));
    pieceVector.emplace_back(factory.constructPiece(PieceType::King));

    return pieceVector;
}
