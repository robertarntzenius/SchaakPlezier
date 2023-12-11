#include <iostream>
#include <string>

#include "board.h"
#include "bitboard.h"
#include "log.h"

/* public */

Board::Board(const std::string& fenString)
    : logger(ChessLogger::getInstance()),
    wKC(true), wQC(true), bKC(true), bQC(true), enPassant(-1), halfMoves(0), fullMoves(0),
    turn    (Color::White),
    wPieces (),
    bPieces (),
    white   ( Bitboard()),
    black   ( Bitboard()),
    wpawns  ( Bitboard()),
    bpawns  ( Bitboard()),
    knights ( Bitboard()),
    bishops ( Bitboard()),
    rooks   ( Bitboard()),
    queens  ( Bitboard()),
    kings   ( Bitboard()),
    notAFile( ~getFileMaskFromSquare(a1) ),
    notBFile( ~getFileMaskFromSquare(b1) ),
    notGFile( ~getFileMaskFromSquare(g1) ),
    notHFile( ~getFileMaskFromSquare(h1) ),
    rank4(getRankMaskFromSquare(a4)),
    rank5(getRankMaskFromSquare(a5)),
    PawnAttacks(2, std::vector<Bitboard>(64))
{
    #ifdef DEBUG
        logger.log("New Board created!");
    #endif

    InitializeFromFEN(fenString);
    FillLookupTables();
    logBoard();
    #ifdef DEBUG
        logBitboards();
        for (Piece &p : wPieces)
        {
            logger.log("", p);
        }
        for (Piece &p : bPieces)
        {
            logger.log("", p);
        }
    #endif
}


Board::~Board() {

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
    // This function doesn't check the legality of a move, that should be done in move generation
    // This function only changes the bitboards depending on the Move it receives

    // Before changing any of the player bitboards, see if the move is a capture
    // if the move is a capture, change necessary opponent bitboards first
    // move.capture is the pType of the captured piece

    // TODO change occupied & empty and other bitboards I might've missed.
    // These arent being used now, so not very important at the moment

    Bitboard *player = m_ColorBitboards.at(turn),
             *opponent = m_ColorBitboards.at(invertColor(turn)),
             *playerPtype = m_pieceTypeBitboards.at(move.piece.type),
             *opponentPtype = m_pieceTypeBitboards.at(move.capture);
    
    if (opponentPtype != nullptr)
    {
        // TODO move pieces in pieceVectors

        opponent->reset(move.target);
        opponentPtype->reset(move.target);
    }

    player->reset(move.piece.square);
    player->set(move.target);
    playerPtype->reset(move.piece.square);
    playerPtype->set(move.target);
    
    #ifdef DEBUG
        logBitboards();

        for (Piece &p : wPieces)
        {
            logger.log("", p);
        }
        for (Piece &p : bPieces)
        {
            logger.log("", p);
        }
    #endif
}

Color Board::switchTurn()
{
    turn = invertColor(turn); 
    return turn;
}

void Board::logBitboards() const
{
    #ifdef DEBUG
        logger.log("white", white);
        logger.log("black", black);
        logger.log("wpawns", wpawns);
        logger.log("bpawns", bpawns);
        logger.log("knights", knights);
        logger.log("bishops", bishops);
        logger.log("rooks", rooks);
        logger.log("queens", queens);
        logger.log("kings", kings);
    #endif
}


/* private */

void Board::InitializeFromFEN(const std::string& fenString) {
    size_t FENIndex = 0;
    int square = 0;
    char ch;

    std::vector<Piece> *pieceVector;

    while ( (FENIndex < fenString.length()) && (square < 64) ) {
        ch = fenString[FENIndex];

        pieceVector = (isupper(ch))? &wPieces : &bPieces;

        if (isalpha(ch)) {
            switch(ch) {
                case 'r': case 'R':
                    rooks.set(square);
                    pieceVector->emplace_back(PieceType::Rook, intToSquare(square));
                    break;
                case 'n': case 'N':
                    knights.set(square);
                    pieceVector->emplace_back(PieceType::Knight, intToSquare(square));
                    break;
                case 'b': case 'B':
                    bishops.set(square);
                    pieceVector->emplace_back(PieceType::Bishop, intToSquare(square));
                    break;
                case 'q': case 'Q':
                    queens.set(square);
                    pieceVector->emplace_back(PieceType::Queen, intToSquare(square));
                    break;
                case 'k': case 'K':
                    kings.set(square);
                    pieceVector->emplace_back(PieceType::King, intToSquare(square));
                    break;
                case 'p':
                    bpawns.set(square);
                    pieceVector->emplace_back(PieceType::bPawn, intToSquare(square));
                    break;
                case 'P':
                    wpawns.set(square);
                    pieceVector->emplace_back(PieceType::wPawn, intToSquare(square));
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
    #ifdef DEBUG
        logger.log("%d %d %d %d %d %d %d %d", turn, halfMoves, fullMoves, enPassant, wKC, wQC, bKC, bQC);
    #endif

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

    _assert(checkBoard());
    #ifdef DEBUG
        logger.log("%d %d %d %d %d %d %d %d", turn, halfMoves, fullMoves, enPassant, wKC, wQC, bKC, bQC);
    #endif
}

void Board::FillLookupTables()
{
    #ifdef DEBUG
        logger.log("Filling Lookup Tables...");
    #endif
    for(int i=BOARDSIZE; i<BOARDSIZE*BOARDSIZE - BOARDSIZE; i++) {
        // white
        PawnAttacks[0][i] = getPawnAttacks(Color::White, intToSquare(i));

        // black
        PawnAttacks[1][i] = getPawnAttacks(Color::Black, intToSquare(i));
    }
    
    #ifdef DEBUG
        logger.log("PawnAttacks white d4", PawnAttacks[0][d4]);
        logger.log("PawnAttacks black d4", PawnAttacks[1][d4]);
    #endif
}


bool Board::checkBoard() {
    #ifdef DEBUG
    // First fill the counts map from wPieces and bPieces
    logger.logHeader("Called CheckBoard()");
    std::map<PieceType, size_t> counts;
    std::vector<Piece>& allPieces = wPieces;
    allPieces.insert(allPieces.end(), bPieces.begin(), bPieces.end());  // Union of wPieces and bPieces

    for (const auto& piece : allPieces) {
        Bitboard* bitboard = m_pieceTypeBitboards.at(piece.type);

        std::string logstring = "[ASSERT] piece location: " + pieceTypeStrings.at(piece.type) + " at " + squareStrings.at(piece.square);
        logger.log(logstring);
        _assert(bitboard->test(piece.square));

        if (counts.count(piece.type) == 0) {
            counts[piece.type] = 1;
        } else {
            counts[piece.type]++;
        }
    }

    // Check material counts
    for (const auto& [pieceType, cnt] : counts) {
        Bitboard* bitboard = m_pieceTypeBitboards.at(pieceType);
        std::string logmsg = "[ASSERT] material count: " + pieceTypeStrings.at(pieceType) + " = " + std::to_string(cnt);
        logger.log(logmsg);

        bool expr = (bitboard->count() == cnt);
        _assert(expr);
    }

    // Check if any bitboards have overlapping bits set
    logger.log("[ASSERT] no overlap global: white, black");
    _assert((white & black) == 0);
    for (auto it1 = m_bitboardNames.begin(); it1 != m_bitboardNames.end(); ++it1) {
        auto& [pName1, bitboard1] = *it1;

        for (auto it2 = std::next(it1); it2 != m_bitboardNames.end(); ++it2) {
            auto& [pName2, bitboard2] = *it2;

            std::string logstring = "[ASSERT] no overlap: pType1: " + pName1 + ", pType2: " + pName2;
            logger.log(logstring);
            _assert((*bitboard1 & *bitboard2) == 0);
        }
    }
    logger.logHeader("Successful CheckBoard()");

    #endif
    return true;
}

const char* getPieceChar(PieceType pType, Color color) {
    switch (pType) {
        case PieceType::wPawn: return "P";
        case PieceType::bPawn: return "p";
        case PieceType::Rook: return (color == Color::White) ? "R" : "r";
        case PieceType::Knight: return (color == Color::White) ? "N" : "n";
        case PieceType::Bishop: return (color == Color::White) ? "B" : "b";
        case PieceType::Queen: return (color == Color::White) ? "Q" : "q";
        case PieceType::King: return (color == Color::White) ? "K" : "k";
        default: return "?";
    }
}

void Board::logBoard() const 
{
    std::ostringstream os;
    char board[BOARDSIZE*BOARDSIZE];

    for (int i = 0; i < BOARDSIZE*BOARDSIZE; i++)
        board[i] = *".";
    
    for (const auto& piece : wPieces) {
        board[piece.square] = *getPieceChar(piece.type, Color::White);
    }
    for (const auto& piece : bPieces) {
        board[piece.square] = *getPieceChar(piece.type, Color::Black);
    }

    for (int i = 0; i < BOARDSIZE*BOARDSIZE; i++) {
        if ((i > 0) && (i % BOARDSIZE == 0))
            os  << std::endl;
        os  << " " << board[i];
    }
    logger.log(os);
}