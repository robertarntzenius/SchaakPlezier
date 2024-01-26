#pragma once

#include "types.h"

#include <iterator>
#include <cstdlib>
#include <vector>


class Bitboard {
    public:
        constexpr Bitboard()
                : bits(0x0)
        {
        }

        ~Bitboard() = default;

        constexpr explicit Bitboard(ulong bits)
                : bits(bits)
        {
        }

        [[nodiscard]] constexpr bool operator==(const Bitboard &other) const {
            return bits == other.bits;
        }

        constexpr Bitboard &operator=(const Bitboard &other) = default;

        [[nodiscard]] constexpr Bitboard operator&(const Bitboard &other) const {
            return Bitboard(bits & other.bits);
        }

        [[nodiscard]] constexpr Bitboard operator|(const Bitboard &other) const {
            return Bitboard(bits | other.bits);
        }

        [[nodiscard]] constexpr Bitboard operator~() const {
            return Bitboard(~bits);
        }

        // NOTE: New and improved: now accepts negative shifts :)
        [[nodiscard]] constexpr Bitboard operator<<(int offset) const{
            if (offset >= 0) {
                return Bitboard(bits << offset);
            }

            return Bitboard(bits >> -offset);
        }

        constexpr Bitboard &set(Square square, bool value = true) {
            ulong mask = 1UL << (BOARD_SIZE - square - 1);

            if (value) {
                bits = bits | mask;
            } else {
                bits = bits & ~mask;
            }
            return *this;
        }

        constexpr Bitboard &set() {
            bits = 0xFFFFFFFFFFFFFFFF;
            return *this;
        }
        constexpr Bitboard &reset() {
            bits = 0;
            return *this;
        }

        // resets lower bits including index itself
        constexpr Bitboard &resetLowerBits(Square square) {
            if (square == NoSquare) {
                return reset();
            }

            ulong lowerBits = (1UL << (BOARD_SIZE - square - 1)) - 1;
            bits = bits & ~lowerBits;
            set(square, false);
            return *this;
        }

        // resets upper bits including index itself
        constexpr Bitboard &resetUpperBits(Square square) {
            if (square == NoSquare) {
                return reset();
            }

            ulong lowerBits = (1UL << (BOARD_SIZE - square - 1)) - 1;
            bits = bits & lowerBits;
            return *this;
        }

        [[nodiscard]] constexpr bool empty() const {
            return bits == 0;
        }

        [[nodiscard]] constexpr size_t count() const {
            size_t count = 0;
            ulong copy = bits;
            while (copy) {
                ++count;
                copy &= copy - 1; // Clear the lowest set bit
            }
            return count;
        }

        [[nodiscard]] constexpr Square getHighestSetBit() const {
            int squareIndex = __builtin_clzll(bits);

            if (squareIndex < a8 || squareIndex > h1) {
                return NoSquare;
            }
            return static_cast<Square>(squareIndex);
        };

        [[nodiscard]] constexpr Square getLowestSetBit() const {
            int squareIndex = BOARD_SIZE - __builtin_ctzll(bits) - 1;

            if (squareIndex < a8 || squareIndex > h1) {
                return NoSquare;
            }
            return static_cast<Square>(squareIndex);
        };

        [[nodiscard]] constexpr bool test(int bitNr) const {
            ulong mask = 1UL << (BOARD_SIZE - bitNr - 1);
            return bits & mask;
        }

        [[nodiscard]] constexpr bool test(Square bitNr) const {
            if (bitNr == NoSquare) {
                return false;
            }

            ulong mask = 1UL << (BOARD_SIZE - bitNr - 1);
            return bits & mask;
        }

        class Iterator : public std::iterator<std::input_iterator_tag, Square> {
            public:
                explicit Iterator(ulong bits) : bitsCopy(bits) {}

                [[nodiscard]] bool operator==(const Iterator& other) const { return bitsCopy == other.bitsCopy; }
                [[nodiscard]] bool operator!=(const Iterator& other) const { return bitsCopy != other.bitsCopy; }

                Iterator& operator++() {
                    bitsCopy &= (bitsCopy - 1);
                    return *this;
                }

                [[nodiscard]] Square operator*() const {
                    return static_cast<Square>(BOARD_SIZE - __builtin_ctzll(bitsCopy) - 1);
                }

            private:
                ulong bitsCopy;
        };

        [[nodiscard]] Iterator begin() const {
            return Iterator(bits);
        }

        [[nodiscard]] static Iterator end() {
            return Iterator(0);
        }

    private:
        ulong bits;
};

