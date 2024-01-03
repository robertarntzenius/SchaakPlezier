#pragma once

#include "definitions.h"
#include "maskgeneration.h"
#include "log.h"


class Board {
    public:
        Board() = delete;
        ~Board() = default;

        explicit Board(const char *FENString = defaultStartingFEN);

        /**
         * @brief Computes and inserts all possible moves from current board state
         *        in the moves vector by reference
         *
         * @param moves
         */
        void getPossibleMoves(std::vector<std::unique_ptr<Move>> &moveVector) const;

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

        // TODO move implementations of movegeneration to its own file
        void generatePawnMoves(std::vector<std::unique_ptr<Move>> &moveVector) const;

        void checkBoardConsistency() const;

        ChessLogger& logger;

        std::unordered_map<Piecetype, Bitboard> piecetypeBitboardMap;
        std::unordered_map<Color, Bitboard> colorBitboardMap;

        std::unordered_map<Square, Piecetype> whitePieceMap;
        std::unordered_map<Square, Piecetype> blackPieceMap;

        Color activePlayer;

        bool wKC, wQC, bKC, bQC;

        Square enPassantSquare;

        int halfMoveClock, fullMoveNumber;

        // het idee is om de PawnAttacks te precomputen en dan een lookup te doen: whitePawnAttacks[square] of blackPawnPushes[square]
        static constexpr std::array<Bitboard, BOARD_SIZE> whitePawnPushLookUp = MaskGeneration::computePawnPushLookUp(White);
        static constexpr std::array<Bitboard, BOARD_SIZE> blackPawnPushLookUp = MaskGeneration::computePawnPushLookUp(Black);
        static constexpr std::array<Bitboard, BOARD_SIZE> whitePawnAttackLookUp = MaskGeneration::computePawnAttackLookUp(White);
        static constexpr std::array<Bitboard, BOARD_SIZE> blackPawnAttackLookUp = MaskGeneration::computePawnAttackLookUp(Black);

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