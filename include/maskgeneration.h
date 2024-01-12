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


    [[nodiscard]] constexpr Bitboard computeKnightScopeFromSquare(Square square) {
        Bitboard playerKnight;
        playerKnight.set(intToSquare(square));

        std::array<Bitboard, NrDirections> directions = {
                (playerKnight & ~computeFileMask(FileH)) << (OFFSET_NORTHEAST + OFFSET_NORTH),
                (playerKnight & ~(computeFileMask(FileG) | computeFileMask(FileH))) << (OFFSET_NORTHEAST + OFFSET_EAST),
                (playerKnight & ~(computeFileMask(FileG) | computeFileMask(FileH))) << (OFFSET_SOUTHEAST + OFFSET_EAST),
                (playerKnight & ~computeFileMask(FileH)) << (OFFSET_SOUTHEAST + OFFSET_SOUTH),
                (playerKnight & ~computeFileMask(FileA)) << (OFFSET_NORTHWEST + OFFSET_NORTH),
                (playerKnight & ~(computeFileMask(FileA) | computeFileMask(FileB))) << (OFFSET_NORTHWEST + OFFSET_WEST),
                (playerKnight & ~(computeFileMask(FileA) | computeFileMask(FileB))) << (OFFSET_SOUTHWEST + OFFSET_WEST),
                (playerKnight & ~computeFileMask(FileA)) << (OFFSET_SOUTHWEST + OFFSET_SOUTH)
        };
        Bitboard knightAttacks;
        for (const auto &direction: directions) {
            knightAttacks = knightAttacks | direction;
        }

        return knightAttacks;
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

    [[nodiscard]] constexpr std::array<std::array<Bitboard, BOARD_SIZE>, NrDirections> computeDirectionalLookUp() {
        std::array<std::array<Bitboard, BOARD_SIZE>, NrDirections> directionalScope;

        // for every square
        for (int squareInt = 0; squareInt < NrSquares; ++squareInt) {
            const Square square = intToSquare(squareInt);
            directionalScope[North][square] = computeFileMask(squareToFile(square)).resetLowerBits(square);
            directionalScope[South][square] = computeFileMask(squareToFile(square)).resetUpperBits(square);
            directionalScope[West][square] = computeRankMask(squareToRank(square)).resetLowerBits(square);
            directionalScope[East][square] = computeRankMask(squareToRank(square)).resetUpperBits(square);
        }

        // TODO: diagonals

        return directionalScope;
    }
}