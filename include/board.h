#pragma once

#include "definitions.h"
#include "bitboard.h"
#include "log.h"

class Board {
    public:
        Board(const std::string& FENString = defaultStartingFEN);
        ~Board();

        std::vector<Move> getPossibleMoves();
        void doMove(Move &move);

        Color switchTurn();

        void logBitboards() const;
    private:
        void InitializeFromFEN(const std::string& fenString);
        void FillLookupTables();

        bool checkBoard();

        Bitboard getRankMask(size_t rank);
        Bitboard getFileMask(size_t file);
        Bitboard getRankMaskFromSquare(Square square);
        Bitboard getFileMaskFromSquare(Square square);

        Bitboard getPawnAttacks(Color color, Square square);
        Bitboard getPawnAttacks(Color color, Bitboard pieceMask);

        Bitboard getPawnPushes(Color color, Square square);

        Bitboard getOccupiedMask () { return (black | white); }
        Bitboard getEmptyMask () { return ~(getOccupiedMask()); }

        ChessLogger& logger;

        bool wKC, wQC, bKC, bQC;
        int enPassant, halfMoves, fullMoves;

        Color turn;

        std::vector<Piece> wPieces;
        std::vector<Piece> bPieces;

        // Global bitboards
        Bitboard white, black, wpawns, bpawns, knights, bishops, rooks, queens, kings;
        const std::unordered_map<PieceType, Bitboard*> m_pieceTypeBitboards = {
            {PieceType::NoType, nullptr},
            {PieceType::wPawn, &wpawns},
            {PieceType::bPawn, &bpawns},
            {PieceType::Knight, &knights},
            {PieceType::Bishop, &bishops},
            {PieceType::Rook, &rooks},
            {PieceType::Queen, &queens},
            {PieceType::King, &kings},
        };

        const std::unordered_map<std::string, Bitboard*> m_bitboardNames = {
            {"wpawns", &wpawns},
            {"bpawns", &bpawns},
            {"knights", &knights},
            {"bishops", &bishops},
            {"rooks", &rooks},
            {"queens", &queens},
            {"kings", &kings}
        };

        const std::unordered_map<Color, Bitboard*> m_ColorBitboards = {
            {Color::White, &white},
            {Color::Black, &black},
        };

        const Bitboard notAFile, notBFile, notGFile, notHFile;
        const Bitboard rank4, rank5;

        // het idee is om de PawnAttacks te precomputen en dan een lookup te doen: PawnAttacks[white][square]
        std::vector<std::vector<Bitboard>> PawnAttacks;

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