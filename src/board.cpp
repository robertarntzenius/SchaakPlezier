#include <iostream>
#include <string>

#include "board.h"
#include "bitboard.h"
#include "log.h"


Board::Board(const std::string& fenString)
    : logger(ChessLogger::getInstance()),
    wKC(true), wQC(true), bKC(true), bQC(true), enPassant(-1), halfMoves(0), fullMoves(0),
    turn(Color::White),
    white   ( Bitboard()),
    black   ( Bitboard()),
    pawns   ( Bitboard()),
    knights ( Bitboard()),
    bishops ( Bitboard()),
    rooks   ( Bitboard()),
    queens  ( Bitboard()),
    kings   ( Bitboard()),
    notAFile( ~getFileMask(0) ),
    notBFile( ~getFileMask(1) ),
    notGFile( ~getFileMask(6) ),
    notHFile( ~getFileMask(7) ),
    rank4(getRankMask(32)),
    rank5(getRankMask(24))
{
    InitializeBitboardsFromFEN(fenString);
    logBitboards();
}

void Board::InitializeBitboardsFromFEN(const std::string& fenString) {
    size_t FENIndex = 0;
    int square = 0;
    char ch;

    while ( (FENIndex < fenString.length()) && (square < 64) ) {
        ch = fenString[FENIndex];
        if (isalpha(ch)) {
            switch(ch) {
                case 'r': case 'R':
                    rooks.set(square);
                break;
                case 'n': case 'N':
                    knights.set(square);
                break;
                case 'b': case 'B':
                    bishops.set(square);
                break;
                case 'q': case 'Q':
                    queens.set(square);
                break;
                case 'k': case 'K':
                    kings.set(square);
                break;
                case 'p': case 'P':
                    pawns.set(square);
                break;
            default:
                // TODO error handling
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

    // FIXME read en passent correctly
    enPassant = (fenString[++FENIndex] == '-') ? -1 : (fenString[FENIndex] - '0');

    logger.log("%d %d %d %d %b %b %b %b", turn, halfMoves, fullMoves, enPassant, wKC, wQC, bKC, bQC);

    FENIndex++;
    while ( (FENIndex < fenString.length()) && (fenString[++FENIndex] != ' ') ) {
        if (!isdigit(fenString[FENIndex])) {
            throw "invalid FENstring\n";
            // FIXME implement real error handling
        }
        halfMoves *= 10;
        halfMoves += fenString[FENIndex] - '0';
    }

    while ( (++FENIndex < fenString.length())) {
        if (!isdigit(fenString[FENIndex])) {
            throw "invalid FENstring\n";
            // FIXME implement real error handling
        }
        fullMoves *= 10;
        fullMoves += fenString[FENIndex] - '0';
    }
    logger.log("%d %d %d %d %b %b %b %b", turn, halfMoves, fullMoves, enPassant, wKC, wQC, bKC, bQC);
}

Board::~Board() {

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

Move *Board::getPossibleMoves()
{
//    Bitboard PawnAttacks = getPawnAttacks();
//    logger.log("PawnAttacks", PawnAttacks);

    getPawnAttacks();
    getPawnPushes();

    return nullptr;
}

Color Board::switchTurn()
{
    turn = (turn == Color::White)? Color::Black : Color::White;
    return turn;
}

Bitboard Board::getPawnAttacks()
{
    Bitboard playerPawns, pawnAttacksWest, pawnAttacksEast;

    if (turn == Color::White) {
        playerPawns = (pawns & white);
        pawnAttacksWest = (playerPawns >> -Offsets::NorthWest) & notHFile;
        pawnAttacksEast = (playerPawns >> -Offsets::NorthEast) & notAFile;
    }
    // NOTE bitshifting with negative values is undefined behaviour in C++
    else {
        playerPawns = (pawns & black);
        pawnAttacksWest = (playerPawns << Offsets::SouthWest) & notHFile;
        pawnAttacksEast = (playerPawns << Offsets::SouthEast) & notAFile;
    }

    logger.log("pawnAttacksWest", pawnAttacksWest);
    logger.log("pawnAttacksEast", pawnAttacksEast);

    return (pawnAttacksWest | pawnAttacksEast);
}

Bitboard Board::getPawnPushes()
{
    Bitboard playerPawns, pawnPushesSingle, pawnPushesDouble;
    
    if (turn == Color::White) {
        playerPawns = (pawns & white);
        pawnPushesSingle = (playerPawns >> -Offsets::North );
        pawnPushesDouble = (playerPawns >> -Offsets::North * 2) & rank4;
    }
    // NOTE bitshifting with negative values is undefined behaviour in C++
    else {
        playerPawns = (pawns & black);
        pawnPushesSingle = (playerPawns << Offsets::South );
        pawnPushesDouble = (playerPawns << Offsets::South * 2) & rank5;
    }

    logger.log("pawnPushesSingle", pawnPushesSingle);
    logger.log("pawnPushesDouble", pawnPushesDouble);

    return (pawnPushesSingle | pawnPushesDouble);
}

void Board::logBitboards()
{
    logger.log("white", white);
    logger.log("black", black);
    logger.log("pawns", pawns);
    logger.log("knights", knights);
    logger.log("bishops", bishops);
    logger.log("rooks", rooks);
    logger.log("queens", queens);
    logger.log("kings", kings);
}
