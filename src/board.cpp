#include <iostream>
#include <string>

#include "board.h"
#include "bitboard.h"
#include "log.h"


Board::Board()
    : logger(ChessLogger::getInstance()),
    wKC(true), wQC(true), bKC(true), bQC(true), enPassant(-1), halfMoves(0), fullMoves(0),
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
    logger.log("white", white);
    logger.log("black", black);
}

Board::Board(const std::string& fenString)
    : logger(ChessLogger::getInstance()),
    wKC(true), wQC(true), bKC(true), bQC(true), enPassant(-1), halfMoves(0), fullMoves(0),
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
    logger.log("white", white);
    logger.log("black", black);
    logger.log("pawns", pawns);
    logger.log("knights", knights);
    logger.log("bishops", bishops);
    logger.log("rooks", rooks);
    logger.log("queens", queens);
    logger.log("kings", kings);
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
    for (size_t i=0; i<BOARDSIZE; i++) {
        fileMask.set(BOARDSIZE * i + file);
    }
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
    logger.log("PawnAttacks", PawnAttacks);
    return nullptr;
}

Color Board::switchTurn() // FIXME is there a better solution for this?
{
    if (turn == Color::White) {
        turn = Color::Black;
        return Color::Black;
    }
    else {
        turn = Color::White;
        return Color::White;
    }
}

Color Board::getTurn()
{
    return turn;
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

Bitboard Board::getPawnPushes() // FIXME refactor the use of offsets here
{
    Bitboard Pawns, PawnPushesSingle, PawnPushesDouble;
    
    if (turn == Color::White) {
        Pawns = (pawns & white);
        PawnPushesSingle = (Pawns >> -Offsets::North ) & empty;
        PawnPushesDouble = (Pawns >> 2*Offsets::North) & empty;
    }
    // NOTE bitshifting with negative values is undefined behaviour in C++
    else {
        Pawns = (pawns & black);
        PawnPushesSingle = (Pawns >> -Offsets::South ) & empty;
        PawnPushesDouble = (Pawns >> 2*Offsets::South) & empty;
    }

    return (PawnPushesSingle | PawnPushesDouble);
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