#pragma once

#include <vector>
#include <string>

#include "bitboard.h"
#include "piece.h"

#define BOARDSIZE 8

struct Move {
    int from;
    int to;
    PieceType pType;
};

class Board {
    public:
        Board();
        Board(const std::string& FENString);
        ~Board();
        Bitboard getFileMask(size_t square);
        Bitboard getRankMask(size_t square);

        int* getPossibleMoves();
        void switchTurn();
        // Bitboard bPawnAttacks();
    private:
        void InitializeBitboardsFromFEN(const std::string& fenString);
        std::vector<Piece*> InitPieces() const;

        bool wKC, wQC, bKC, bQC;
        int enPassant, halfMoves, fullMoves;

        Color turn;

        // FIXME aparte piece structuur zo (moet gewoon 1 van elke type) 
        std::vector<Piece*> pieces;
        
        // Global constant bitboards
        const Bitboard empty = Bitboard( (unsigned long)0 );
        const Bitboard universe = Bitboard( ~empty );

        Bitboard white, black, pawns, knights, bishops, rooks, queens, kings;
        Bitboard notAFile, notBFile, notCFile, notDFile, notEFile, notFFile, notGFile, notHFile;
        // NOTE I initialized all of these, but are probably not necessary
        
        Bitboard getPawnAttacks();


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