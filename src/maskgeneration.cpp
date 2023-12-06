#include "board.h"

Bitboard Board::getPawnAttacks(Color color, Square square)
{
    Bitboard playerPawn;
    playerPawn.set(square);

    return getPawnAttacks(color, playerPawn);
}

Bitboard Board::getPawnAttacks(Color color, Bitboard pieceMask)
{
    Bitboard pawnAttacksWest, pawnAttacksEast;

    if (color == Color::White) {
        pieceMask = (pawns & white);
        pawnAttacksWest = (pieceMask >> -Offsets::NorthWest) & notHFile;
        pawnAttacksEast = (pieceMask >> -Offsets::NorthEast) & notAFile;
    }
    // NOTE bitshifting with negative values is undefined behaviour in C++
    else {
        pieceMask = (pawns & black);
        pawnAttacksWest = (pieceMask << Offsets::SouthWest) & notHFile;
        pawnAttacksEast = (pieceMask << Offsets::SouthEast) & notAFile;
    }

    return (pawnAttacksWest | pawnAttacksEast);
}


Bitboard Board::getPawnPushes(Color color, Square square)
{
    Bitboard playerPawn, singlePush, doublePush;

    playerPawn.set(square);

    if (color == Color::White) {
        singlePush = (playerPawn >> -Offsets::North );
        doublePush = (playerPawn >> -Offsets::North * 2) & rank4;
    }
    // NOTE bitshifting with negative values is undefined behaviour in C++
    else {
        singlePush = (playerPawn << Offsets::South );
        doublePush = (playerPawn << Offsets::South * 2) & rank5;
    }

    return (singlePush | doublePush);
}

Bitboard Board::getRankMask(size_t rank)
{
    // get rankMask with top rank == 0 and bottom == BOARDSIZE - 1
    Bitboard rankMask;
    if ((rank > BOARDSIZE)) {
        std::cerr << "Invalid rank index: " << rank << std::endl;
    }
    else {
        rankMask = Bitboard( ( (1UL << BOARDSIZE) -1) << rank*BOARDSIZE);
    }
    return rankMask;
}

Bitboard Board::getFileMask(size_t file)
{
    // get fileMask with left file == 0 and right == BOARDSIZE - 1
    Bitboard fileMask;
    if ((file > BOARDSIZE)) {
        std::cerr << "Invalid file index: " << file << std::endl;
    }
    else {
        for (size_t i=0; i<BOARDSIZE; i++) {
            fileMask.set(BOARDSIZE * i + file);
        }
    }
    return fileMask;
}

Bitboard Board::getRankMaskFromSquare(Square square)
{
    // get rankMask with topleft=0 and bottomright = BOARDSIZE*BOARDSIZE - 1
    size_t rank = square / BOARDSIZE;
    Bitboard rankMask( ( (1UL << BOARDSIZE) -1) << rank*BOARDSIZE);
    return rankMask;
}

Bitboard Board::getFileMaskFromSquare(Square square)
{
    // get fileMask with topleft = 0 and bottomright = BOARDSIZE*BOARDSIZE - 1
    size_t file = square % BOARDSIZE;
    Bitboard fileMask;
    for (size_t i=0; i<BOARDSIZE; i++) {
        fileMask.set(BOARDSIZE * i + file);
    }
    return fileMask;
}