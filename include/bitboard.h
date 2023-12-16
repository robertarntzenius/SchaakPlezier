#pragma once
#include "definitions.h"

#include <iostream>
#include <bitset>

#define BOARDSIZE 8

class Bitboard : public std::bitset<BOARDSIZE * BOARDSIZE> {
    public:
        Bitboard();
        Bitboard(unsigned long bits);
        Bitboard(std::bitset<BOARDSIZE*BOARDSIZE>);
        ~Bitboard();
        const std::vector<Square> getIndices() const;


        // Custom operator<< for Bitboard
        friend std::ostream& operator<<(std::ostream& os, const Bitboard& bitboard) {
            for (int rank = 0; rank < BOARDSIZE; rank++) {
                for (int file = 0; file < BOARDSIZE; file++) {
                    int square = rank * BOARDSIZE + file;
                    if (bitboard.test(square)) {
                        os << "1 ";
                    } else {
                        os << "0 ";
                    }
                }
                os << std::endl;
            }
            return os;
        }
    private:
};