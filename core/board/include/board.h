#pragma once

#include "definitions.h"
#include "maskgeneration.h"
#include "movegeneration.h"
#include "log.h"

#include <algorithm>

class Board {
    public:
        ~Board() = default;

        Board(const char *FENString = defaultStartingFEN, const std::string &logFile = "Schaakplezier.log");

        /**
         * @brief Computes and inserts all possible moves from current board state
         *        in the moves vector by reference
         *
         * @param moves
         */
        void getPossibleMoves(std::vector<Move> &moveVector, BoardState &copyState);

        /**
         * @brief Performs move from current board state
         *
         * @param move
         */
        void doMove(const Move &move);
        /**
         * @brief Takes back the last  move from current board state
         *
         * @param move
         */
        void undoMove(const Move &move, const BoardState &state);
        /**
         * @brief returns whether specific player is in check from the current board state
         *
         * @param player
         * @return bool
         */
        [[nodiscard]] bool inCheck(Color player) const;

        /**
         * @brief returns whether active player is in check from the current board state
         *
         * @return bool
         */
        [[nodiscard]] bool inCheck() const;

        /**
         * @brief Logs current board state to logger in ASCII chessboard
         */
        void logBoard(LogLevel logLevel) const;

        /**
         * @brief Logs all Bitboard members to logger
         */
        void logBitboards() const;

        /**
         * @brief Get active player
         */
        Color getActivePlayer() const { return boardState.activePlayer; };

        friend std::ostream& operator<<(std::ostream &os, const Board &board);

        // useful functions for testing
        void setLogLevel(LogLevel logLevel);

        void checkBoardConsistency() const;
        
        bool checkInsufficientMaterial() const;
        bool checkFiftyMoveRule() const;
        bool checkThreeFoldRepetition() const;
    private:
        /* Methods*/
        void InitializeFromFEN(const char *FENString);
        void movePiece(Color player, Piecetype pieceType, Square fromSquare, Square toSquare);

        /* MoveGen */
        void generatePawnMoves(std::vector<Move> &moveVector, Square fromSquare) const;
        void generateKnightMoves(std::vector<Move> &moveVector, Square fromSquare) const;
        void generateKingMoves(std::vector<Move> &moveVector, Square fromSquare) const;
        void generateSliderMoves(std::vector<Move> &moveVector, Square fromSquare, Piecetype pieceType) const;

        void generatePawnPushes(std::vector<Move> &moveVector, Square fromSquare) const;
        void generatePawnCaptures(std::vector<Move> &moveVector, Square fromSquare) const;
        void generateCastleMove(std::vector<Move> &moveVector, CastlingSide side) const;
        
        /* MaskGen */
        Bitboard getAttacksFromSlider(Square fromSquare, Piecetype piecetype) const;
        Bitboard getPlayerAttackMask(Color player) const;
        
        /* Member variables */
        ChessLogger& logger;
        BoardState boardState;

        std::array<Bitboard, NrPiecetypes> piecetypeBitboards;
        std::array<Bitboard, NrColors> colorBitboards;

        std::array<std::unordered_map<Square, Piecetype>, NrColors> pieceMaps;
        
        /* static lookup arrays*/
        static constexpr std::array<std::array<Bitboard, BOARD_SIZE>, NrColors> pawnPushLookUp = {
                MaskGeneration::computePawnPushLookUp(White),
                MaskGeneration::computePawnPushLookUp(Black)
        };
        static constexpr std::array<std::array<Bitboard, BOARD_SIZE>, NrColors> pawnAttackLookUp = {
                MaskGeneration::computePawnAttackLookUp(White),
                MaskGeneration::computePawnAttackLookUp(Black)
        };
        static constexpr std::array<Bitboard, BOARD_SIZE> knightScopeLookUp =
                MaskGeneration::computeKnightScopeLookUp();
        
        static constexpr std::array<Bitboard, BOARD_SIZE> kingScopeLookUp = 
                MaskGeneration::computeKingScopeLookUp();

        static constexpr std::array<std::array<Bitboard, BOARD_SIZE>, NrDirections> directionalLookUp =
                MaskGeneration::computeDirectionalLookUp();

        static constexpr std::array<Bitboard, NrColors> finalRank = {
                MaskGeneration::computeRankMask(Rank8),
                MaskGeneration::computeRankMask(Rank1)
        };

        static constexpr Bitboard darkSquares = MaskGeneration::computeDarkSquares();
        static constexpr Bitboard lightSquares = ~MaskGeneration::computeDarkSquares();
};