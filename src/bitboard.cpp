#include "bitboard.h"

Bitboard::Bitboard(unsigned long bits) 
    : std::bitset<BOARDSIZE*BOARDSIZE>(bits)
{
}

Bitboard::~Bitboard()
{
}
