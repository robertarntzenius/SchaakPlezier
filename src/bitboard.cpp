#include "bitboard.h"

Bitboard::Bitboard() 
    : std::bitset<BOARDSIZE * BOARDSIZE>(0)
{
}

Bitboard::Bitboard(unsigned long bits)
    : std::bitset<BOARDSIZE * BOARDSIZE>(bits)
{
}

Bitboard::Bitboard(std::bitset<BOARDSIZE * BOARDSIZE> bits)
    : std::bitset<BOARDSIZE*BOARDSIZE>(bits)
{
}

Bitboard::~Bitboard()
{
}