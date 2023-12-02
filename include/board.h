#pragma once

#include <vector>
#include <string>

#include "bitboard.h"
#include "piece.h"
#include "log.h"

#define BOARDSIZE 8

enum Square {
    a8=0, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
};

struct Move {
    Square from;
    Square to;
    PieceType pType;
    PieceType capture;

    Move(Square from, Square to, PieceType pType, PieceType capture) 
    : from(from), to(to), pType(pType), capture(capture) {}
};

class Board {
    public:
        Board(const std::string& FENString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        ~Board();

        std::vector<Move> getPossibleMoves();
        void doMove(Move &move);

        Color switchTurn();
    private:
        void InitializeBitboardsFromFEN(const std::string& fenString);
        void FillLookupTables();

        Bitboard getPawnPushes();
        Bitboard getPawnAttacks();

        Bitboard getRankMask(size_t rank);
        Bitboard getFileMask(size_t file);
        Bitboard getRankMaskFromSquare(Square square);
        Bitboard getFileMaskFromSquare(Square square);
        void logBitboards();

        Bitboard getPawnAttacksFromSquare(Square square, Color color);



        ChessLogger& logger;

        bool wKC, wQC, bKC, bQC;
        int enPassant, halfMoves, fullMoves;

        Color turn;

        // FIXME aparte piece structuur zo (moet gewoon 1 van elke type) 
//        std::vector<Piece*> pieces;
        
        Bitboard white, black, pawns, knights, bishops, rooks, queens, kings;
        
        // Global bitboards
        Bitboard occupied, empty;
        Bitboard notAFile, notBFile, notGFile, notHFile;
        Bitboard rank4, rank5;

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