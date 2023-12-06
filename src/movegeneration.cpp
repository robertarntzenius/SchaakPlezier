#include "board.h"

Bitboard Board::getPawnAttacksFromSquare(Square square, Color color)
{
    Bitboard playerPawn, pawnAttacksWest, pawnAttacksEast;
    playerPawn.set(square);

    if (color == Color::White) {
        pawnAttacksWest = (playerPawn >> -Offsets::NorthWest) & notHFile;
        pawnAttacksEast = (playerPawn >> -Offsets::NorthEast) & notAFile;
    }
        // NOTE bitshifting with negative values is undefined behaviour in C++
    else {
        pawnAttacksWest = (playerPawn << Offsets::SouthWest) & notHFile;
        pawnAttacksEast = (playerPawn << Offsets::SouthEast) & notAFile;
    }
    return Bitboard(pawnAttacksEast | pawnAttacksWest);
}

Bitboard Board::getPawnPushesFromSquare(Square square, Color color)
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


    logger.log("singlePush", singlePush);
    logger.log("doublePush", doublePush);

    return (singlePush | doublePush);
}