#pragma once

#include "types.h"

#include <cstdlib>
#include <vector>

// NOTE: making this functionally constexpr was a hassle, but apparently it needs
//       to be fully defined in the header

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

        constexpr Bitboard &set(int bitNr, bool value = true) {
            ulong mask = 1UL << (BOARD_SIZE - bitNr - 1);

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

        [[nodiscard]] constexpr int count() const {
            int count = 0;
            ulong copy = bits;
            while (copy) {
                ++count;
                copy &= copy - 1; // Clear the lowest set bit
            }
            return count;
        }

        [[nodiscard]] constexpr bool test(int bitNr) const {
            ulong mask = 1UL << (BOARD_SIZE - bitNr - 1);
            return bits & mask;
        }

        [[nodiscard]] std::vector<Square> getIndices() const {
            std::vector<Square> indices;
            ulong copy = bits;
            while (copy) {
                int square = __builtin_ctzll(copy); // Count trailing zeros using a built-in function
                indices.push_back(static_cast<Square>(square));
                copy &= copy - 1; // Clear the lowest set bit
            }
            return indices;
        }

    private:
        ulong bits;
};
