#include <iostream>
#include <string>

#include "board.h"
#include "bitboard.h"

Board::Board()
    : wKC(true), wQC(true), bKC(true), bQC(true), enPassant(-1), halfMoves(0), fullMoves(0),
    turn    ( Color::White ), 
    pieces  ( InitPieces() ),
    white   ( Bitboard( ((1UL << 16) -1) << 48 ) ), 
    black   ( Bitboard( (1UL << 16) -1) ),
    pawns   ( Bitboard( (((1UL << 8) -1) << 8) | (((1UL << 8) -1) << 48) ) ), 
    knights ( Bitboard( (1UL << 1) | (1UL << 6) | (1UL << 57) | (1UL << 62) ) ), 
    bishops ( Bitboard( (1UL << 2) | (1UL << 5) | (1UL << 58) | (1UL << 61)) ), 
    rooks   ( Bitboard( (1UL) | (1UL << 7) | (1UL << 56) | (1UL << 63)) ), 
    queens  ( Bitboard( (1UL << 3) | (1UL << 59) ) ),
    kings   ( Bitboard( (1UL << 4) | (1UL << 60) ) ),
    notAFile( ~getFileMask(0) ),
    notBFile( ~getFileMask(1) ),
    notCFile( ~getFileMask(2) ),
    notDFile( ~getFileMask(3) ),
    notEFile( ~getFileMask(4) ),
    notFFile( ~getFileMask(5) ),
    notGFile( ~getFileMask(6) ),
    notHFile( ~getFileMask(7) )
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
    kings   ( Bitboard( 0UL )),
    notAFile( ~getFileMask(0) ),
    notBFile( ~getFileMask(1) ),
    notCFile( ~getFileMask(2) ),
    notDFile( ~getFileMask(3) ),
    notEFile( ~getFileMask(4) ),
    notFFile( ~getFileMask(5) ),
    notGFile( ~getFileMask(6) ),
    notHFile( ~getFileMask(7) )
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
    size_t FENIndex = 0;

    while ( (FENIndex < fenString.length()) && (square < 64) ) {
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
    while ( (FENIndex < fenString.length()) && (fenString[++FENIndex] != ' ') ) {
        if (!isdigit(fenString[FENIndex])) {
            std::cerr << "invalid FENstring" << std::endl;
            // FIXME implement real error handling
        }
        halfMoves *= 10;
        halfMoves += fenString[FENIndex] - '0';
    }

    while ( (FENIndex < fenString.length())) {
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

Bitboard Board::getFileMask(size_t square)
{
    size_t file = square % 8;
    Bitboard fileMask;
    for (size_t i=0; i<BOARDSIZE*BOARDSIZE; i++) {
        if (i % 8 == 0)
            fileMask.set(i);
    }
    fileMask = fileMask << file;
    return fileMask;
}

Bitboard Board::getRankMask(size_t square)
{
    size_t rank = (int) square/8;
    Bitboard rankMask( ( (1UL << 8) -1) << rank*8);
    return rankMask;
}

int *Board::getPossibleMoves()
{
    Bitboard PawnAttacks = getPawnAttacks();
    std::cout << PawnAttacks << std::endl;

    return nullptr;
}

void Board::switchTurn() // FIXME is there a better solution for this?
{
    if (turn == Color::White) 
        turn = Color::Black;
    else {
        turn = Color::White;
    }
    // std::cout << "switched to " << turn << std::endl; 
}

Bitboard Board::getPawnAttacks() // FIXME refactor the use of offsets here
{
    Bitboard Pawns, PawnAttacks, PawnAttacksWest, PawnAttacksEast;
    
    if (turn == Color::White) {
        Pawns = (pawns & white);
        PawnAttacksWest = (Pawns >> -Offsets::NorthWest ) & notHFile;
        PawnAttacksEast = (Pawns >> -Offsets::NorthEast) & notAFile;
    } 
    // NOTE bitshifting with negative values is undefined behaviour in C++
    else {
        Pawns = (pawns & black);
        PawnAttacksWest = (Pawns << Offsets::SouthWest ) & notHFile;
        PawnAttacksEast = (Pawns << Offsets::SouthEast) & notAFile;
    }
    return (PawnAttacksWest | PawnAttacksEast);
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