#pragma once

#include "definitions.h"
#include "maskgeneration.h"
#include "movegeneration.h"
#include "log.h"


class Board {
    public:
        ~Board() = default;

        Board(const char *FENString = defaultStartingFEN);

        /**
         * @brief Computes and inserts all possible moves from current board state
         *        in the moves vector by reference
         *
         * @param moves
         */
        void getPossibleMoves(std::vector<Move> &moveVector) const;

        /**
         * @brief Performs move from current board state
         *
         * @param move
         */
        void doMove(const Move *move);

        /**
         * @brief returns whether specific player is in check from the current board state
         *
         * @param player
         * @return bool
         */
        [[nodiscard]] bool inCheck(Color player) const;

        /**
         * @brief Logs current board state to logger in ASCII chessboard
         */
        void logBoard() const;

        /**
         * @brief Logs all Bitboard members to logger
         */
        void logBitboards() const;
    private:
        void InitializeFromFEN(const char *FENString);

        void generatePawnPushes(std::vector<Move> &moveVector, Square fromSquare) const;
        void generatePawnCaptures(std::vector<Move> &moveVector, Square fromSquare) const;
        
        void generateKnightMoves(std::vector<Move> &moveVector, Square fromSquare) const;
//        void generateBishopMoves(std::vector<Move> &moveVector, Square fromSquare) const;
        void generateRookMoves(std::vector<Move> &moveVector, Square fromSquare) const;
//        void generateQueenMoves(std::vector<Move> &moveVector, Square fromSquare) const;
//        void generateKingMoves(std::vector<Move> &moveVector, Square fromSquare) const;



        void checkBoardConsistency() const;

        ChessLogger& logger;

        std::array<Bitboard, NrPiecetypes> piecetypeBitboards;
        std::array<Bitboard, NrColors> colorBitboards;

        std::array<std::unordered_map<Square, Piecetype>, NrColors> pieceMaps;

        Color activePlayer;

        bool wKC, wQC, bKC, bQC;

        Square enPassantSquare;

        int halfMoveClock, fullMoveNumber;

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

        static constexpr std::array<std::array<Bitboard, BOARD_SIZE>, NrDirections> directionalLookUp =
                MaskGeneration::computeDirectionalLookUp();


        static constexpr std::array<Bitboard, NrColors> finalRank = {
                MaskGeneration::computeRankMask(Rank8),
                MaskGeneration::computeRankMask(Rank1)
        };
        // Bitboard bPawnAttacks();
        // Bitboards worden vaak geprecompute en in een array gezet for quick lookup
        // bv KingMoves[sq] = bitboard van de goede squareIndex
        // blackAttacks(sq) whiteAttacks(sq) (add all attacked squares in one bitboard)
            // dan later legalKingMoves = Kingmoves[sq] & (!black/whiteAttacks)
        
        // NON-SLIDING PIECES (psuedolegal moves)
        // TODO add bitboards / getterFunctions that return bitboards for 
            // empty
            //  white/black:
                //  pawns
                //  PawnAttacksWest (using emtpy & NotHFile)
                //  PawnAttacksEast (using emtpy & NotAFile) 
            //  kingMoves[sq]
            //  knightsAttacks[sq] (using emtpy & NotAFile & NotABFile & NotGHFile & NotHFile)
        
        // SLIDING PIECES (psuedolegal moves)
        // TODO Add directions enum to generate bitboard masks for north east etc
        // TODO add rankMask(sq), fileMask(sq), diagonalMask(sq), antidiagonalMask(sq) functions
        // RookAttacks[sq]   = rankMask[sq]     | fileMask[sq];
        // BishopAttacks[sq] = DiagonalMask[sq] | AntiDiagonalMask[sq];
        // QueenAttacks[sq]  = RookAttacks[sq] | BishopAttacks[sq];


        // TODO XRayAttacks (to handle pinned pieces)
        // TODO blockers (to find legalmoves from psuedolegal moves for SLIDING PIECES)
};