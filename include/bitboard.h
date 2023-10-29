#pragma once

#include <iostream>
#include <bitset>

#define BOARDSIZE 8

class Bitboard : public std::bitset<BOARDSIZE * BOARDSIZE> {
    public:
        Bitboard(unsigned long bits);
        ~Bitboard();

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