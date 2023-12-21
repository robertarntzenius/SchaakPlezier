#include <iostream>
#include <string>

#include "board.h"
#include "bitboard.h"
#include "log.h"

/* public */

Board::Board(const std::string& fenString)
    : logger(ChessLogger::getInstance()),
    wKC(false), wQC(false), bKC(false), bQC(false), enPassant(noSquare), halfMoves(0), fullMoves(0),
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
    rank2(getRankMaskFromSquare(a2)),
    rank4(getRankMaskFromSquare(a4)),
    rank5(getRankMaskFromSquare(a5)),
    rank7(getRankMaskFromSquare(a7)),
    PawnAttacks(2, std::vector<Bitboard>(64)),
    PawnPushes(2, std::vector<Bitboard>(64))
{
    #ifdef DEBUG
        logger.log("New Board created!");
    #endif

    InitializeFromFEN(fenString);
    FillLookupTables();
    logBoard();
    #ifdef DEBUG
        // logBitboards();
        // for (Piece &p : wPieces)
        // {
        //     logger.log("", p);
        // }
        // for (Piece &p : bPieces)
        // {
        //     logger.log("", p);
        // }
    #endif
}


Board::~Board() {

}


std::vector<Move> Board::getPossibleMoves()
{
    #ifdef DEBUG
    logger.logHeader("getPossibleMoves()");
    logBoard();
    #endif

    std::vector<Move> allMoves;
    std::vector<Move> pawnMoves = generatePawnMoves();

    allMoves.insert(allMoves.end(), pawnMoves.begin(), pawnMoves.end());
    return allMoves;
}

std::vector<Move> Board::generatePawnMoves() {
    #ifdef DEBUG
    logger.logHeader("generatePawnMoves()");
    #endif
    
    std::vector<Move> pawnMoves;
    
    int color = (turn == Color::White) ? 0 : 1;
    PieceType playerPawnType = (turn == Color::White) ? wPawn : bPawn;
    PieceType opponentPawnType = (turn == Color::White) ? bPawn : wPawn;
    const Bitboard *playerPawns = (turn == Color::White) ? &wpawns : &bpawns;
    const Bitboard *opponent = (turn == Color::White) ? &black : &white;
    const Bitboard empty = Bitboard(black | white).flip();


    // Generate Capture Moves
    for (Square fromSquare : playerPawns->getIndices()) {
        // doing the comparison here skips the for loop for many pawns
        Bitboard validAttacks = (PawnAttacks[color][fromSquare] & *opponent);
        for (Square toSquare : validAttacks.getIndices()) {
            #ifdef DEBUG
            Move move(fromSquare, toSquare, playerPawnType, findPiece(toSquare));
            logger.log("CAPTURE");
            logger.log(move);
            #endif
            pawnMoves.emplace_back(fromSquare, toSquare, playerPawnType, findPiece(toSquare));
        }

        // enPassant
        // cant use validAttacks because the enPassant square is empty by definition
        if ((enPassant != noSquare) && (PawnAttacks[color][fromSquare].test(enPassant))) {
            #ifdef DEBUG
            Move move(fromSquare, enPassant, playerPawnType, opponentPawnType);
            logger.log("ENPASSANT");
            logger.log(move);
            #endif
            Square capturedPieceSquare = (turn == White) ? intToSquare(enPassant + BOARDSIZE) : intToSquare(enPassant - BOARDSIZE);
            pawnMoves.emplace_back(fromSquare, enPassant, playerPawnType, findPiece(capturedPieceSquare));
            // TODO: make sure to handle this move correctly when actually making it
        }
    }

    // Generate Push moves
    for (Square fromSquare : playerPawns->getIndices()) { // for every pawn
        for (Square toSquare : PawnPushes[color][fromSquare].getIndices()) { // for every attacked square of that pawn
            
            // Double pushes
            if ( (abs(fromSquare - toSquare) == 2*BOARDSIZE)
                    && (empty.test(intToSquare((toSquare+fromSquare)/2))) // if one square in front is empty
                    && (empty.test(toSquare)) )
            { 
                #ifdef DEBUG
                Move move(fromSquare, toSquare, playerPawnType, NoType);
                logger.log("DOUBLE PUSH");
                logger.log(move);
                #endif
                pawnMoves.emplace_back(fromSquare, toSquare, playerPawnType, NoType, (intToSquare((toSquare+fromSquare)/2)));

                // TODO set enpassant square or at least pass it along with the move?
            }

            // Single pushes
            else if ( (abs(fromSquare - toSquare) == BOARDSIZE)
                && empty.test(toSquare))
            {
                #ifdef DEBUG
                Move move(fromSquare, toSquare, playerPawnType, NoType);
                logger.log("SINGLE PUSH");
                logger.log(move);
                #endif
                pawnMoves.emplace_back(fromSquare, toSquare, playerPawnType, NoType);
            }
        }
    }
    return pawnMoves;
}


bool Board::makeMove(Move &move)
{
    // This function checks the legality of a move at the end by checking
    // if the king is in check in the resulting position return -1
    // if not in check return 0;

    // This function only changes the bitboards depending on the Move it receives

    // Before changing any of the player bitboards, see if the move is a capture
    // if the move is a capture, change necessary opponent bitboards first
    // move.capture is the pType of the captured piece

    // TODO change occupied & empty and other bitboards I might've missed.
    // These arent being used now, so not very important at the moment

    Bitboard *player = m_ColorBitboards.at(turn),
             *opponent = m_ColorBitboards.at(invertColor(turn)),
             *playerPtype = m_pieceTypeBitboards.at(move.piece.type),
             *opponentPtype = m_pieceTypeBitboards.at(move.capturedPiece.type);
    std::vector<Piece> *playerPieces = (turn == White) ? &wPieces : &bPieces;
    std::vector<Piece> *opponentPieces = (turn == White) ? &bPieces : &wPieces;

    Square previousEnPassant = enPassant;
    setEnPassant(move.enPassant);


    if (opponentPtype != nullptr)
    {
        opponentPieces->erase(
            std::remove(opponentPieces->begin(), opponentPieces->end(), move.capturedPiece)
            , opponentPieces->end());

            opponent->reset(move.target);
            opponentPtype->reset(move.target);
    }

    for (auto& piece : *playerPieces) 
    {
        if (piece == move.piece ) {
            piece.square = move.target;  // Update the square to the destination square
            break;  // Break out of the loop once the piece is found and updated
        }
    }

    // TODO implement castling

    player->reset(move.piece.square);
    player->set(move.target);
    playerPtype->reset(move.piece.square);
    playerPtype->set(move.target);
    
    #ifdef DEBUG
        // logBitboards();

        for (Piece &piece : wPieces)
        {
            logger.log(piece);
        }
        for (Piece &piece : bPieces)
        {
            logger.log(piece);
        }
    #endif

    _assert(checkBoard());

    if (inCheck()) { // TODO implement
        setEnPassant(previousEnPassant);
        return false;
    }
    
    return true;
}

void Board::setEnPassant(Square square)
{
    enPassant = square;
}

Color Board::switchTurn()
{
    #ifdef DEBUG
    logger.log("switchTurn()");
    #endif
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
    while (fenString[FENIndex] != ' ')
    {
        switch(fenString[FENIndex])
        {
            case 'K': wKC = true; break;
            case 'Q': wKC = true; break;
            case 'k': bKC = true; break;
            case 'q': bKC = true; break;
        }
        FENIndex++;
    }

    enPassant = (fenString[++FENIndex] == '-') ? noSquare : squareFromString(fenString.substr(FENIndex, 2));
    FENIndex += (enPassant == noSquare) ? 0 : 2;
    
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
        PawnPushes[0][i] = getPawnPushes(Color::White, intToSquare(i));
        // black
        PawnAttacks[1][i] = getPawnAttacks(Color::Black, intToSquare(i));
        PawnPushes[1][i] = getPawnPushes(Color::Black, intToSquare(i));
        

    }
    
    #ifdef DEBUG
        logger.log("PawnAttacks white d4", PawnAttacks[0][d4]);
        logger.log("PawnAttacks black d4", PawnAttacks[1][d4]);
        logger.log("PawnPushes white d2", PawnPushes[0][d2]);
        logger.log("PawnPushes black d7", PawnPushes[1][d7]);
    #endif
}

bool Board::checkBoard(bool quiet/* = true */) const {
    #ifdef DEBUG
    // First fill the counts map from wPieces and bPieces
    std::map<PieceType, size_t> counts;

    std::vector<Piece> allPieces;
    allPieces.insert(allPieces.end(), wPieces.begin(), wPieces.end());
    allPieces.insert(allPieces.end(), bPieces.begin(), bPieces.end());

    for (const auto& piece : allPieces) {
        Bitboard* bitboard = m_pieceTypeBitboards.at(piece.type);

        std::string logstring = "[ASSERT] piece location: " + pieceTypeStrings.at(piece.type) + " at " + squareStrings.at(piece.square);
        if (!quiet) logger.log(logstring);
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
        if (!quiet) logger.log(logmsg);

        bool expr = (bitboard->count() == cnt);
        _assert(expr);
    }

    // Check if any bitboards have overlapping bits set
    if (!quiet) logger.log("[ASSERT] no overlap global: white, black");
    _assert((white & black) == 0);
    for (auto it1 = m_bitboardNames.begin(); it1 != m_bitboardNames.end(); ++it1) {
        auto& [pName1, bitboard1] = *it1;

        for (auto it2 = std::next(it1); it2 != m_bitboardNames.end(); ++it2) {
            auto& [pName2, bitboard2] = *it2;

            std::string logstring = "[ASSERT] no overlap: pType1: " + pName1 + ", pType2: " + pName2;
            if (!quiet) logger.log(logstring);
            _assert((*bitboard1 & *bitboard2) == 0);
        }
    }
    #endif
    return true;
}

bool Board::inCheck() const
{
    // TODO: implement
    return false;
}


Piece Board::findPiece(Square toSquare)
{
    // _assert(checkBoard());
    std::vector<Piece> *pieces = (turn == Color::White) ? &bPieces : &wPieces;
    for (auto& piece : *pieces) {
        if (piece.square == toSquare) {
            return piece;
        }
    }
    return Piece();
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

Square squareFromString(const std::string& algebraic) {
    for (const auto& entry : squareStrings) {
        if (entry.second == algebraic) {
            return entry.first;
        }
    }
    return noSquare;  // Return noSquare if the string is not found
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

    for (int rank = 0; rank < BOARDSIZE; rank++) {
        os << BOARDSIZE-rank << " ";
        for (int file = 0; file <  BOARDSIZE; file++) {
            int index = rank * BOARDSIZE + (file);
            os << " " << board[index];
        }
        os  << std::endl;
    }
    os << std::endl <<  "   a b c d e f g h" << std::endl;
    os << "enPassant: " << squareStrings.at(enPassant) << std::endl;
    logger.log(os);
}