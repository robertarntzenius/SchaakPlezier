#include <iostream>
#include <string>

#include "board.h"
#include "bitboard.h"
#include "log.h"


Board::Board(const std::string& fenString)
    : logger(ChessLogger::getInstance()),
    wKC(true), wQC(true), bKC(true), bQC(true), enPassant(-1), halfMoves(0), fullMoves(0),
    turn(Color::White),
    white   ( Bitboard()),
    black   ( Bitboard()),
    pawns   ( Bitboard()),
    knights ( Bitboard()),
    bishops ( Bitboard()),
    rooks   ( Bitboard()),
    queens  ( Bitboard()),
    kings   ( Bitboard()),
    occupied( Bitboard(black & white)),
    empty   ( Bitboard(~occupied)),
    notAFile( ~getFileMaskFromSquare(a1) ),
    notBFile( ~getFileMaskFromSquare(b1) ),
    notGFile( ~getFileMaskFromSquare(g1) ),
    notHFile( ~getFileMaskFromSquare(h1) ),
    rank4(getRankMaskFromSquare(a4)),
    rank5(getRankMaskFromSquare(a5)),
    PawnAttacks(2, std::vector<Bitboard>(64))
{
    logger.log("New Board created!");
    InitializeBitboardsFromFEN(fenString);
    FillLookupTables();
    logBitboards();
}

void Board::InitializeBitboardsFromFEN(const std::string& fenString) {
    size_t FENIndex = 0;
    int square = 0;
    char ch;

    while ( (FENIndex < fenString.length()) && (square < 64) ) {
        ch = fenString[FENIndex];
        if (isalpha(ch)) {
            switch(ch) {
                case 'r': case 'R':
                    rooks.set(square);
                break;
                case 'n': case 'N':
                    knights.set(square);
                break;
                case 'b': case 'B':
                    bishops.set(square);
                break;
                case 'q': case 'Q':
                    queens.set(square);
                break;
                case 'k': case 'K':
                    kings.set(square);
                break;
                case 'p': case 'P':
                    pawns.set(square);
                break;
            default:
                // TODO error handling
                std::cerr << "Robert fucked up" << std::endl;
            }
            if(isupper(ch)) {
                white.set(square);
            }
            else {
                black.set(square);
            }
            square++;
        }

        if (isdigit(ch)) {
            square += ch - '0';
        }
        
        FENIndex++;
    }

    turn = (fenString[++FENIndex] == 'w') ? Color::White : Color::Black;
    FENIndex += 2;
    wKC = (fenString[FENIndex++] != '-');
    wQC = (fenString[FENIndex++] != '-');
    bKC = (fenString[FENIndex++] != '-');
    bQC = (fenString[FENIndex++] != '-');

    // FIXME read en passent correctly
    enPassant = (fenString[++FENIndex] == '-') ? -1 : (fenString[FENIndex] - '0');

    logger.log("%d %d %d %d %d %d %d %d", turn, halfMoves, fullMoves, enPassant, wKC, wQC, bKC, bQC);

    FENIndex++;
    while ( (FENIndex < fenString.length()) && (fenString[++FENIndex] != ' ') ) {
        if (!isdigit(fenString[FENIndex])) {
            throw "invalid FENstring\n";
            // FIXME implement real error handling
        }
        halfMoves *= 10;
        halfMoves += fenString[FENIndex] - '0';
    }

    while ( (++FENIndex < fenString.length())) {
        if (!isdigit(fenString[FENIndex])) {
            throw "invalid FENstring\n";
            // FIXME implement real error handling
        }
        fullMoves *= 10;
        fullMoves += fenString[FENIndex] - '0';
    }
    logger.log("%d %d %d %d %d %d %d %d", turn, halfMoves, fullMoves, enPassant, wKC, wQC, bKC, bQC);
}

Board::~Board() {

}

void Board::FillLookupTables()
{
    logger.log("Filling Lookup Tables...");
    for(int i=BOARDSIZE; i<BOARDSIZE*BOARDSIZE - BOARDSIZE; i++) {
        // white
        PawnAttacks[0][i] = getPawnAttacksFromSquare(static_cast<Square>(i), Color::White);
        
        // black
        PawnAttacks[1][i] = getPawnAttacksFromSquare(static_cast<Square>(i), Color::Black);
    }

    logger.log("PawnAttacks white d4", PawnAttacks[0][d4]);
    logger.log("PawnAttacks black d4", PawnAttacks[1][d4]);
}

Bitboard Board::getRankMask(size_t rank)
{
    // get rankMask with top rank == 0 and bottom == BOARDSIZE - 1
    Bitboard rankMask;
    if ((rank > BOARDSIZE)) {
        std::cerr << "Invalid rank index: " << rank << std::endl;
    }
    else {
        rankMask = Bitboard( ( (1UL << BOARDSIZE) -1) << rank*BOARDSIZE);
    }
    return rankMask;
}

Bitboard Board::getFileMask(size_t file)
{
    // get fileMask with left file == 0 and right == BOARDSIZE - 1
    Bitboard fileMask;
    if ((file > BOARDSIZE)) {
        std::cerr << "Invalid file index: " << file << std::endl;
    }
    else {
        for (size_t i=0; i<BOARDSIZE; i++) {
            fileMask.set(BOARDSIZE * i + file);
        }
    }
    return fileMask;
}

Bitboard Board::getRankMaskFromSquare(Square square)
{
    // get rankMask with topleft=0 and bottomright = BOARDSIZE*BOARDSIZE - 1
    size_t rank = square / BOARDSIZE;
    Bitboard rankMask( ( (1UL << BOARDSIZE) -1) << rank*BOARDSIZE);
    return rankMask;
}

Bitboard Board::getFileMaskFromSquare(Square square)
{
    // get fileMask with topleft = 0 and bottomright = BOARDSIZE*BOARDSIZE - 1
    size_t file = square % BOARDSIZE;
    Bitboard fileMask;
    for (size_t i=0; i<BOARDSIZE; i++) {
        fileMask.set(BOARDSIZE * i + file);
    }
    return fileMask;
}

std::vector<Move> Board::getPossibleMoves()
{
    std::vector<Move> moves, testmoves;
    testmoves.emplace_back(a2, e8, wPawn, King);


    for (const auto& move : testmoves)
    {
        moves.push_back(move);
    }
    return moves;
}

void Board::doMove(Move &move)
{
    // This function doesnt check the legality of a move, that should be done in move generation
    // This function only changes the bitboards depending on the Move it receives

    // Before changing any of the player bitboards, see if the move is a capture
    // if the move is a capture, change necessary opponent bitboards first
    // move.capture is the pType of the captured piece

    // TODO change occupied & empty and other bitboards I mightve missed.
    // These arent being used now, so not very important at the moment

    Bitboard *player = m_ColorBitboards.at(turn),
             *opponent = m_ColorBitboards.at(invertColor(turn)),
             *playerPtype = m_pieceTypeBitboards.at(move.pType),
             *opponentPtype = m_pieceTypeBitboards.at(move.capture);
    
    if (opponentPtype != nullptr)
    {
        opponent->reset(move.to);
        opponentPtype->reset(move.to);
    }

    player->reset(move.from);
    player->set(move.to);
    playerPtype->reset(move.from);
    playerPtype->set(move.to);
    logBitboards();
}

Color Board::switchTurn()
{
    turn = invertColor(turn); 
    return turn;
}

Bitboard Board::getPawnAttacks()
{
    Bitboard playerPawns, pawnAttacksWest, pawnAttacksEast;
    
    if (turn == Color::White) {
        playerPawns = (pawns & white);
        pawnAttacksWest = (playerPawns >> -Offsets::NorthWest) & notHFile;
        pawnAttacksEast = (playerPawns >> -Offsets::NorthEast) & notAFile;
    }
    // NOTE bitshifting with negative values is undefined behaviour in C++
    else {
        playerPawns = (pawns & black);
        pawnAttacksWest = (playerPawns << Offsets::SouthWest) & notHFile;
        pawnAttacksEast = (playerPawns << Offsets::SouthEast) & notAFile;
    }

    // logger.log("pawnAttacksWest", pawnAttacksWest);
    // logger.log("pawnAttacksEast", pawnAttacksEast);

    return (pawnAttacksWest | pawnAttacksEast);
}

Bitboard Board::getPawnPushes()
{
    Bitboard playerPawns, pawnPushesSingle, pawnPushesDouble;
    
    if (turn == Color::White) {
        playerPawns = (pawns & white);
        pawnPushesSingle = (playerPawns >> -Offsets::North );
        pawnPushesDouble = (playerPawns >> -Offsets::North * 2) & rank4;
    }
    // NOTE bitshifting with negative values is undefined behaviour in C++
    else {
        playerPawns = (pawns & black);
        pawnPushesSingle = (playerPawns << Offsets::South );
        pawnPushesDouble = (playerPawns << Offsets::South * 2) & rank5;
    }

    // logger.log("pawnPushesSingle", pawnPushesSingle);
    // logger.log("pawnPushesDouble", pawnPushesDouble);

    return (pawnPushesSingle | pawnPushesDouble);
}

void Board::logBitboards()
{
    logger.log("white", white);
    logger.log("black", black);
    logger.log("pawns", pawns);
    logger.log("knights", knights);
    logger.log("bishops", bishops);
    logger.log("rooks", rooks);
    logger.log("queens", queens);
    logger.log("kings", kings);
}

Bitboard Board::getPawnAttacksFromSquare(Square square, Color color)
{
    Bitboard playerPawn, pawnAttacksWest, pawnAttacksEast;
    playerPawn.set(square);
    
    if (color == Color::White) {
        pawnAttacksWest = (playerPawn >> -Offsets::NorthWest) & notHFile;
        pawnAttacksEast = (playerPawn >> -Offsets::NorthEast) & notAFile;
    }
    // NOTE bitshifting with negative values is undefined behaviour in C++
    else {
        pawnAttacksWest = (playerPawn << Offsets::SouthWest) & notHFile;
        pawnAttacksEast = (playerPawn << Offsets::SouthEast) & notAFile;
    }
    return Bitboard(pawnAttacksEast | pawnAttacksWest);
}
