#pragma once

#include "definitions.h"
#include "maskgeneration.h"
#include "movegeneration.h"
#include "log.h"

#include <algorithm>
#include <stack>


class Board {
    public:
        ~Board() = default;
        Board(const char *FENString = defaultStartingFEN, const std::string &logFile = "Schaakplezier.log");

        /**
         * @brief initialize the board from the FENstring provided.
         *
         * @param FENString
         */
        void initFromFEN(const char *FENString);

        /**
         * @brief clears the board members and set boardState to default.
         *
         * @param
         */
        void clearBoard();

        /**
         * @brief Computes and inserts all possible moves from current board state
         *        in the moves vector by reference.
         * @note It is recommended to reserve space in the moveVector before calling this function
         *
         * @param moveVector
         */
        void getPossibleMoves(std::vector<Move> &moveVector);

        /**
         * @brief Computes and inserts all capture & check moves from current board state
         *        in the moves vector by reference
         *
         * @param moveVector
         */
        void getLoudMoves(std::vector<Move> &moveVector, bool &noLegalMoves);

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
        void undoMove();
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
         * @brief returns the hash of the current boardState by constructing it from 0.
         *
         * @return bool
         */
        [[nodiscard]] uint64_t computeHashFromScratch();

        /**
         * @brief Logs current board state to logger in ASCII chessboard
         */
        void logBoard(LogLevel logLevel) const;

        /**
         * @brief Logs all Bitboard members to logger
         */
        void logBitboards() const;

        friend std::ostream& operator<<(std::ostream &os, const Board &board);

        // useful functions for testing
        void validate() const;
        std::pair<bool, std::vector<std::string>> try_validate() const;
        PieceInfo pieceAtSquare(Square square) const;
        void addPiece(Color color, Piecetype type, Square square);
        void removePiece(Color color, Piecetype type, Square square);
        bool checkInsufficientMaterial() const;
        bool checkFiftyMoveRule() const;
        bool checkThreeFoldRepetition() const;

        /* Setters & Getters */
        void setLogLevel(LogLevel logLevel) { logger.setLogLevel(logLevel); }

        // Expose getters and setters for gui
        [[nodiscard]] Color getActivePlayer() const;
        [[nodiscard]] GameResult getGameResult(bool noLegalMoves) const;
        [[nodiscard]] BoardState getBoardState() const;
        [[nodiscard]] std::unordered_map<Square, Piecetype> getPieceMap(Color color) const;
        [[nodiscard]] std::array<Bitboard, NrPiecetypes> getPiecetypeBitboards() const;
        [[nodiscard]] std::array<Bitboard, NrColors> getColorBitboards() const;
        [[nodiscard]] std::stack<MoveCommand> getHistory() const;

        void setBoardState(const BoardState &copyState);
        void getPiecetypeBitboards(const std::array<Bitboard, NrPiecetypes> &copyPiecetypeBitboards);
        void getColorBitboards(const std::array<Bitboard, NrColors> &copyColorBitboards );
        void setPieceMaps(const std::array<std::unordered_map<Square, Piecetype>, NrColors> &copyMaps);

    private:
        /* Methods*/
        void movePiece(Color player, Piecetype pieceType, Square fromSquare, Square toSquare, bool updateHash = true);
        void removeCastlingRights(Square square);

        /* MoveGen */
        void getPseudoLegalMoves(std::vector<Move> &moveVector) const;

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

        std::stack<MoveCommand> history;
        std::unordered_map<uint64_t, size_t> repetitionTable;

        std::array<Bitboard, NrPiecetypes> piecetypeBitboards;
        std::array<Bitboard, NrColors> colorBitboards;

        std::array<std::unordered_map<Square, Piecetype>, NrColors> pieceMaps;

        /* Hashing */
        std::array<std::array<std::array<uint64_t, NrPiecetypes>, NrColors>, NrSquares> zobristPieceTable;
        std::array<uint64_t, NrCastlingRights> zobristCastlingTable;
        uint64_t zobristActivePlayer;

        void initZobristTables();
        void hashPiece(Color player, Piecetype pieceType, Square square);
        void hashCastlingRight(CastlingSide side);
        void hashActivePlayer();

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

template <>
class std::hash<Board>
{
public:
   uint64_t operator()(const Board& board) const
   {
      return board.getBoardState().hash;
   }
};
