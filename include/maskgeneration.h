#pragma once

#include "definitions.h"

namespace MaskGeneration {

    [[nodiscard]] constexpr Bitboard computeFileMask(File file) {
        Bitboard fileMask;

        for (int i = 0; i < BOARD_DIMENSIONS; i++) {
            const Square square = intToSquare(BOARD_DIMENSIONS * i + file);
            fileMask.set(square);
        }
        return fileMask;
    }

    [[nodiscard]] constexpr Bitboard computeRankMask(Rank rank) {
        return Bitboard(0xFFUL << (BOARD_DIMENSIONS - rank - 1) * BOARD_DIMENSIONS);
    }

    [[nodiscard]] constexpr Bitboard computeDiagonalsMask(int square, DirectionalOffset offset) {
        
        Bitboard scope;

        while (true) {
            if ( ((square - offset) < a8) || ((square - offset) > h1) ) {
                return scope;
            }
            
            switch (offset)
            {
            case OFFSET_NORTHEAST:
            case OFFSET_SOUTHEAST:
                if (computeFileMask(FileH).test(square)) {
                    return scope;
                }
                break;
            case OFFSET_NORTHWEST:
            case OFFSET_SOUTHWEST:
                if (computeFileMask(FileA).test(square)) {
                    return scope;
                }
                break;
                break;
            default:
                throw std::invalid_argument("Only diagonal directions allowed");
            }

            square -= offset;
            scope.set(intToSquare(square));
        }
    }

    [[nodiscard]] constexpr std::array<Bitboard, BOARD_SIZE> computePawnPushLookUp(Color color) {
        std::array<Bitboard, BOARD_SIZE> pawnPushes{};
        Bitboard singlePush, doublePush;

        DirectionalOffset directionalOffset = NO_OFFSET;
        Bitboard doublePushRank;

        switch (color) {
            case White:
                directionalOffset = OFFSET_NORTH;
                doublePushRank = computeRankMask(Rank4);
                break;
            case Black:
                directionalOffset = OFFSET_SOUTH;
                doublePushRank = computeRankMask(Rank5);
                break;
            default:
                throw std::invalid_argument("Invalid color: " + std::to_string(color));
        }

        // NOTE: technically doesn't have to compute anything for ranks 1 and 8
        for (int square = 0; square < NrSquares; square++) {
            Bitboard playerPawn;
            playerPawn.set(intToSquare(square));

            singlePush = (playerPawn << directionalOffset);
            doublePush = (playerPawn << directionalOffset * 2) & doublePushRank;

            pawnPushes[square] = (singlePush | doublePush);
        }

        return pawnPushes;
    }

    [[nodiscard]] constexpr std::array<Bitboard, BOARD_SIZE> computePawnAttackLookUp(Color color) {
        std::array<Bitboard, BOARD_SIZE> pawnAttacks;

        DirectionalOffset westOffset = NO_OFFSET;
        DirectionalOffset eastOffset = NO_OFFSET;

        switch (color) {
            case White:
                westOffset = OFFSET_NORTHWEST;
                eastOffset = OFFSET_NORTHEAST;
                break;
            case Black:
                westOffset = OFFSET_SOUTHWEST;
                eastOffset = OFFSET_SOUTHEAST;
                break;
            default:
                throw std::invalid_argument("Invalid color: " + std::to_string(color));
        }

        for (int squareInt = 0; squareInt < NrSquares; ++squareInt) {
            const Square square = intToSquare(squareInt);
            Bitboard playerPawn;

            playerPawn.set(square);

            Bitboard pawnAttacksWest = (playerPawn << westOffset) & ~computeFileMask(FileH);
            Bitboard pawnAttacksEast = (playerPawn << eastOffset) & ~computeFileMask(FileA);

            pawnAttacks[square] = (pawnAttacksWest | pawnAttacksEast);
        }

        return pawnAttacks;
    }

    [[nodiscard]] constexpr Bitboard computePawnAttacksFromBitboard(Bitboard bitboard, Color color) {
        DirectionalOffset westOffset = NO_OFFSET;
        DirectionalOffset eastOffset = NO_OFFSET;

        switch (color) {
            case White:
                westOffset = OFFSET_NORTHWEST;
                eastOffset = OFFSET_NORTHEAST;
                break;
            case Black:
                westOffset = OFFSET_SOUTHWEST;
                eastOffset = OFFSET_SOUTHEAST;
                break;
            default:
                throw std::invalid_argument("Invalid color: " + std::to_string(color));
        }

        Bitboard pawnAttacksWest = (bitboard << westOffset) & ~computeFileMask(FileH);
        Bitboard pawnAttacksEast = (bitboard << eastOffset) & ~computeFileMask(FileA);

        return (pawnAttacksWest | pawnAttacksEast);
    }

    [[nodiscard]] constexpr Bitboard computeKnightScopeFromBitboard(Bitboard bitboard) {
        std::array<Bitboard, NrDirections> directions = {
            (bitboard & ~computeFileMask(FileH)) << (OFFSET_NORTHEAST + OFFSET_NORTH),
            (bitboard & ~(computeFileMask(FileG) | computeFileMask(FileH))) << (OFFSET_NORTHEAST + OFFSET_EAST),
            (bitboard & ~(computeFileMask(FileG) | computeFileMask(FileH))) << (OFFSET_SOUTHEAST + OFFSET_EAST),
            (bitboard & ~computeFileMask(FileH)) << (OFFSET_SOUTHEAST + OFFSET_SOUTH),
            (bitboard & ~computeFileMask(FileA)) << (OFFSET_NORTHWEST + OFFSET_NORTH),
            (bitboard & ~(computeFileMask(FileA) | computeFileMask(FileB))) << (OFFSET_NORTHWEST + OFFSET_WEST),
            (bitboard & ~(computeFileMask(FileA) | computeFileMask(FileB))) << (OFFSET_SOUTHWEST + OFFSET_WEST),
            (bitboard & ~computeFileMask(FileA)) << (OFFSET_SOUTHWEST + OFFSET_SOUTH)
        };
        Bitboard knightAttacks;
        for (const auto &direction: directions) {
            knightAttacks = knightAttacks | direction;
        }
        return knightAttacks;
    }
    
    [[nodiscard]] constexpr Bitboard computeKnightScopeFromSquare(Square square) {
        Bitboard playerKnight;
        playerKnight.set(square);
        return computeKnightScopeFromBitboard(playerKnight);
    }


    [[nodiscard]] constexpr std::array<Bitboard, BOARD_SIZE> computeKnightScopeLookUp() {
        std::array<Bitboard, BOARD_SIZE> knightScope;

        // for every square
        for (int squareInt = 0; squareInt < NrSquares; ++squareInt) {
            const Square square = intToSquare(squareInt);
            knightScope[squareInt] = computeKnightScopeFromSquare(square);
        }

        return knightScope;
    }

    [[nodiscard]] constexpr Bitboard computeKingScopeFromBitboard(Bitboard bitboard) {
        std::array<Bitboard, NrDirections> directions = {
            (bitboard & ~computeRankMask(Rank8)) << (OFFSET_NORTH),
            (bitboard & ~computeRankMask(Rank1)) << (OFFSET_SOUTH),
            (bitboard & ~computeFileMask(FileH)) << (OFFSET_EAST),
            (bitboard & ~computeFileMask(FileA)) << (OFFSET_WEST),

            (bitboard & (~computeRankMask(Rank8) & ~computeFileMask(FileH))) << (OFFSET_NORTHEAST),
            (bitboard & (~computeRankMask(Rank8) & ~computeFileMask(FileA))) << (OFFSET_NORTHWEST),
            (bitboard & (~computeRankMask(Rank1) & ~computeFileMask(FileH))) << (OFFSET_SOUTHEAST),
            (bitboard & (~computeRankMask(Rank1) & ~computeFileMask(FileA))) << (OFFSET_SOUTHWEST)
        };
        Bitboard kingAttacks;
        for (const auto &direction: directions) {
            kingAttacks = kingAttacks | direction;
        }
        return kingAttacks;
    }

    [[nodiscard]] constexpr Bitboard computeKingScopeFromSquare(Square square) {
        Bitboard playerKing;
        playerKing.set(square);
        return computeKingScopeFromBitboard(playerKing);
    }

    [[nodiscard]] constexpr std::array<Bitboard, BOARD_SIZE> computeKingScopeLookUp() {
        std::array<Bitboard, BOARD_SIZE> kingScope;

        // for every square
        for (int squareInt = 0; squareInt < NrSquares; ++squareInt) {
            const Square square = intToSquare(squareInt);
            kingScope[squareInt] = computeKingScopeFromSquare(square);
        }
        return kingScope;
    }

    [[nodiscard]] constexpr std::array<std::array<Bitboard, BOARD_SIZE>, NrDirections> computeDirectionalLookUp() {
        std::array<std::array<Bitboard, BOARD_SIZE>, NrDirections> directionalScope;

        // for every square
        for (int squareInt = 0; squareInt < NrSquares; ++squareInt) {
            const Square square = intToSquare(squareInt);
            directionalScope[North][square] = computeFileMask(squareToFile(square)).resetLowerBits(square);
            directionalScope[South][square] = computeFileMask(squareToFile(square)).resetUpperBits(square);
            directionalScope[West][square] = computeRankMask(squareToRank(square)).resetLowerBits(square);
            directionalScope[East][square] = computeRankMask(squareToRank(square)).resetUpperBits(square);

            directionalScope[NorthEast][square] = computeDiagonalsMask(squareInt, OFFSET_NORTHEAST);
            directionalScope[NorthWest][square] = computeDiagonalsMask(squareInt, OFFSET_NORTHWEST);
            directionalScope[SouthEast][square] = computeDiagonalsMask(squareInt, OFFSET_SOUTHEAST);
            directionalScope[SouthWest][square] = computeDiagonalsMask(squareInt, OFFSET_SOUTHWEST);
        }
        return directionalScope;
    }

} // namespace MaskGeneration