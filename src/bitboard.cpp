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

const std::vector<Square> Bitboard::getIndices() const
{
    std::vector<Square> indices;
    for (int square = 0; square < BOARDSIZE*BOARDSIZE; square++) {
        if (test(square)) {
            indices.push_back(intToSquare(square));
        }
    }
    return indices;
}
