#pragma once

#include "definitions.h"

namespace MaskGeneration {

    [[nodiscard]] constexpr Bitboard computeFileMask(int file) {
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

        DirectionalOffset directionalOffset = NoOffset;
        Bitboard doublePushRank;

        switch (color) {
            case White:
                directionalOffset = North;
                doublePushRank = computeRankMask(Rank4);
                break;
            case Black:
                directionalOffset = South;
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

    [[nodiscard]] constexpr Bitboard computePawnPushesFromBitboard(Bitboard bitboard, Color color) {
        std::array<Bitboard, BOARD_SIZE> pawnPushes{};
        Bitboard singlePush, doublePush;

        DirectionalOffset directionalOffset = NoOffset;
        Bitboard enPassantRank;

        switch (color) {
            case White:
                directionalOffset = North;
                enPassantRank = computeRankMask(Rank4);
                break;
            case Black:
                directionalOffset = South;
                enPassantRank = computeRankMask(Rank5);
                break;
            default:
                throw std::invalid_argument("Invalid color: " + std::to_string(color));
        }

        // NOTE: technically doesn't have to compute anything for ranks 1 and 8
        singlePush = (bitboard << directionalOffset);
        doublePush = (bitboard << directionalOffset * 2) & enPassantRank;

        return (singlePush | doublePush);
    }

    [[nodiscard]] constexpr std::array<Bitboard, BOARD_SIZE> computePawnAttackLookUp(Color color) {
        std::array<Bitboard, BOARD_SIZE> pawnAttacks;

        DirectionalOffset westOffset = NoOffset;
        DirectionalOffset eastOffset = NoOffset;

        switch (color) {
            case White:
                westOffset = NorthWest;
                eastOffset = NorthEast;
                break;
            case Black:
                westOffset = SouthWest;
                eastOffset = SouthEast;
                break;
            default:
                throw std::invalid_argument("Invalid color: " + std::to_string(color));
        }

        for (int squareInt = 0; squareInt < NrSquares; ++squareInt) {
            const Square square = intToSquare(squareInt);
            Bitboard playerPawn;

            playerPawn.set(square);

            Bitboard pawnAttacksWest = (playerPawn << westOffset) & ~computeFileMask(7);
            Bitboard pawnAttacksEast = (playerPawn << eastOffset) & ~computeFileMask(0);

            pawnAttacks[square] = (pawnAttacksWest | pawnAttacksEast);
        }

        return pawnAttacks;
    }

    [[nodiscard]] constexpr Bitboard computePawnAttacksFromBitboard(Bitboard bitboard, Color color) {
        DirectionalOffset westOffset = NoOffset;
        DirectionalOffset eastOffset = NoOffset;

        switch (color) {
            case White:
                westOffset = NorthWest;
                eastOffset = NorthEast;
                break;
            case Black:
                westOffset = SouthWest;
                eastOffset = SouthEast;
                break;
            default:
                throw std::invalid_argument("Invalid color: " + std::to_string(color));
        }

        Bitboard pawnAttacksWest = (bitboard << westOffset) & ~computeFileMask(7);
        Bitboard pawnAttacksEast = (bitboard << eastOffset) & ~computeFileMask(0);

        return (pawnAttacksWest | pawnAttacksEast);
    }
}


