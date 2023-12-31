#include "board.h"

/* public */

Board::Board(const std::string& fenString)
    : logger(ChessLogger::getInstance()),
      piecetypeBitboardMap{
              {Pawn,    {}},
              {Knight,  {}},
              {Bishop,  {}},
              {Rook,    {}},
              {Queen,   {}},
              {King,    {}},
      },
      colorBitboardMap{
              {White, {}},
              {Black, {}},
      },
      whitePieceMap{},
      blackPieceMap{},
      activePlayer(Color::White),
      wKC(false), wQC(false), bKC(false), bQC(false),
      enPassantSquare(NoSquare),
      halfMoveClock(0),
      fullMoveNumber(0)
{
    #ifdef DEBUG
        logger.log("New Board created!");
    #endif

    InitializeFromFEN(fenString);
    logBoard();
    logBitboards();
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

//std::vector<Move> Board::getPossibleMoves()
//{
//    #ifdef DEBUG
//    logger.logHeader("getPossibleMoves()");
//    logBoard();
//    #endif
//
//    std::vector<Move> allMoves;
//    std::vector<Move> pawnMoves = generatePawnMoves();
//
//    allMoves.insert(allMoves.end(), pawnMoves.begin(), pawnMoves.end());
//    return allMoves;
//}

//std::vector<Move> Board::generatePawnMoves() {
//    #ifdef DEBUG
//    logger.logHeader("generatePawnMoves()");
//    #endif
//
//    std::vector<Move> pawnMoves;
//
//    int color = (activePlayer == Color::White) ? 0 : 1;
//    Piecetype playerPawnType = (activePlayer == Color::White) ? wPawn : bPawn;
//    Piecetype opponentPawnType = (activePlayer == Color::White) ? bPawn : wPawn;
//    const Bitboard *playerPawns = (activePlayer == Color::White) ? &wpawns : &bpawns;
//    const Bitboard *opponent = (activePlayer == Color::White) ? &black : &white;
//    const Bitboard empty = Bitboard(black | white).flip();
//
//
//    // Generate Capture Moves
//    for (Square fromSquare : getIndices(*playerPawns)) {
//        // doing the comparison here skips the for loop for many pawns
//        Bitboard validAttacks = (PawnAttacks[color][fromSquare] & *opponent);
//        for (Square toSquare : getIndices(validAttacks)) {
//            #ifdef DEBUG
//            Move move(fromSquare, toSquare, playerPawnType, findPiece(toSquare));
//            logger.log("CAPTURE");
//            logger.log(move);
//            #endif
//            pawnMoves.emplace_back(fromSquare, toSquare, playerPawnType, findPiece(toSquare));
//        }
//
//        // enPassantSquare
//        // cant use validAttacks because the enPassantSquare square is empty by definition
//        if ((enPassantSquare != NoSquare) && (PawnAttacks[color][fromSquare].test(enPassantSquare))) {
//            #ifdef DEBUG
//            Move move(fromSquare, enPassantSquare, playerPawnType, opponentPawnType);
//            logger.log("ENPASSANT");
//            logger.log(move);
//            #endif
//            Square capturedPieceSquare = (activePlayer == White) ? intToSquare(enPassantSquare + BOARD_DIMENSIONS) : intToSquare(enPassantSquare - BOARD_DIMENSIONS);
//            pawnMoves.emplace_back(fromSquare, enPassantSquare, playerPawnType, findPiece(capturedPieceSquare));
//            // TODO: make sure to handle this move correctly when actually making it
//        }
//    }
//
//    // Generate Push moves
//    for (Square fromSquare : getIndices(*playerPawns)) { // for every pawn
//        for (Square toSquare : getIndices(PawnPushes[color][fromSquare])) { // for every attacked square of that pawn
//
//            // Double pushes
//            if ( (abs(fromSquare - toSquare) == 2 * BOARD_DIMENSIONS)
//                    && (empty.test(intToSquare((toSquare+fromSquare)/2))) // if one square in front is empty
//                    && (empty.test(toSquare)) )
//            {
//                #ifdef DEBUG
//                Move move(fromSquare, toSquare, playerPawnType, NoType);
//                logger.log("DOUBLE PUSH");
//                logger.log(move);
//                #endif
//                pawnMoves.emplace_back(fromSquare, toSquare, playerPawnType, NoType, (intToSquare((toSquare+fromSquare)/2)));
//
//                // TODO set enpassant square or at least pass it along with the move?
//            }
//
//            // Single pushes
//            else if ( (abs(fromSquare - toSquare) == BOARD_DIMENSIONS)
//                && empty.test(toSquare))
//            {
//                #ifdef DEBUG
//                Move move(fromSquare, toSquare, playerPawnType, NoType);
//                logger.log("SINGLE PUSH");
//                logger.log(move);
//                #endif
//                pawnMoves.emplace_back(fromSquare, toSquare, playerPawnType, NoType);
//            }
//        }
//    }
//    return pawnMoves;
//}


//bool Board::makeMove(Move &move)
//{
//    // This function checks the legality of a move at the end by checking
//    // if the king is in check in the resulting position return -1
//    // if not in check return 0;
//
//    // This function only changes the bitboards depending on the Move it receives
//
//    // Before changing any of the player bitboards, see if the move is a capture
//    // if the move is a capture, change necessary opponent bitboards first
//    // move.capture is the pType of the captured piece
//
//    // TODO change occupied & empty and other bitboards I might've missed.
//    // These arent being used now, so not very important at the moment
//
//    Bitboard *player = m_ColorBitboards.at(activePlayer),
//             *opponent = m_ColorBitboards.at(invertColor(activePlayer)),
//             *playerPtype = m_pieceTypeBitboards.at(move.piece.type),
//             *opponentPtype = m_pieceTypeBitboards.at(move.capturedPiece.type);
//    std::vector<Piece> *playerPieces = (activePlayer == White) ? &wPieces : &bPieces;
//    std::vector<Piece> *opponentPieces = (activePlayer == White) ? &bPieces : &wPieces;
//
//    Square previousEnPassant = enPassantSquare;
//    setEnPassant(move.enPassant);
//
//
//    if (opponentPtype != nullptr)
//    {
//        opponentPieces->erase(
//            std::remove(opponentPieces->begin(), opponentPieces->end(), move.capturedPiece)
//            , opponentPieces->end());
//
//            opponent->reset(move.capturedPiece.square);
//            opponentPtype->reset(move.capturedPiece.square);
//    }
//
//    for (auto& piece : *playerPieces)
//    {
//        if (piece == move.piece ) {
//            piece.square = move.target;  // Update the square to the destination square
//            break;  // Break out of the loop once the piece is found and updated
//        }
//    }
//
//    // TODO implement castling
//
//    player->reset(move.piece.square);
//    player->set(move.target);
//    playerPtype->reset(move.piece.square);
//    playerPtype->set(move.target);
//
//    #ifdef DEBUG
//        // logBitboards();
//
//        for (Piece &piece : wPieces)
//        {
//            logger.log(piece);
//        }
//        for (Piece &piece : bPieces)
//        {
//            logger.log(piece);
//        }
//    #endif
//
//    _assert(checkBoardConsistency());
//
//    if (inCheck()) { // TODO implement
//        setEnPassant(previousEnPassant);
//        return false;
//    }
//
//    return true;
//}

//void Board::setEnPassant(Square square)
//{
//    enPassantSquare = square;
//}

//Color Board::switchTurn()
//{
//    #ifdef DEBUG
//    logger.log("switchTurn()");
//    #endif
//    activePlayer = invertColor(activePlayer);
//    return activePlayer;
//}

void Board::logBitboards() const
{
    #ifdef DEBUG
        for (const auto &entry : colorBitboardMap) {
            logger.log(colorStringMap.at(entry.first).c_str(), entry.second);
        }
        for (const auto &entry : piecetypeBitboardMap) {
            logger.log(piecetypeStringMap.at(entry.first).c_str(), entry.second);
        }
    #endif
}


/* private */

void Board::InitializeFromFEN(const std::string &fenString)
{
    std::string boardString;
    char activeColorChar;
    std::string castlingRightsString;
    std::string enPassantSquareString;

    std::istringstream iss(fenString);
    iss >> boardString >> activeColorChar >> castlingRightsString
        >> enPassantSquareString >> halfMoveClock >> fullMoveNumber;

    int square = 0;

    for (const char &c : boardString) {
        switch (c) {
            case '/':
                break;
            case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8':
                square += c - '0';
                break;
            default:
                Piecetype type = charPiecetypeMap.at(c);
                piecetypeBitboardMap.at(type).set(square);

                if (isupper(c)) {
                    colorBitboardMap.at(White).set(square);
                    whitePieceMap[intToSquare(square)] = type;
                } else {
                    colorBitboardMap.at(Black).set(square);
                    blackPieceMap[intToSquare(square)] = type;
                }

                square++;
                break;
        }
    }

    activePlayer = charColorMap.at(activeColorChar);

    for (const char &c : castlingRightsString) {
        switch(c) {
            case 'K': wKC = true; break;
            case 'Q': wQC = true; break;
            case 'k': bKC = true; break;
            case 'q': bQC = true; break;
            default:
                throw std::invalid_argument("Invalid char found in FEN parser during initialization of castling rights: " + std::to_string(c));
        }
    }

    enPassantSquare = stringSquareMap.at(enPassantSquareString);

//    _assert(checkBoardConsistency());
    #ifdef DEBUG
        logger.log("%d %d %d %d %d %d %d %d", activePlayer, halfMoveClock, fullMoveNumber, enPassantSquare, wKC, wQC, bKC, bQC);
    #endif
}

//constexpr void Board::InitializeFromFEN(const std::string& fenString) {
//    size_t FENIndex = 0;
//    int square = 0;
//    char ch;
//
//    std::unordered_map<Square, Piece> *pieceMapPtr;
//
//    while ( (FENIndex < fenString.length()) && (square < 64) ) {
//        ch = fenString[FENIndex];
//
//        pieceMapPtr = (isupper(ch))? &whitePieceMap : &blackPieceMap;
//
//        if (isalpha(ch)) {
//            switch(ch) {
//                case 'r': case 'R':
//                    Bitboard rooks = piecetypeBitboardMap.at(Rook);
//                    rooks.set(square);
//                    pieceVector->emplace_back(Piecetype::Rook, intToSquare(square));
//                    break;
//                case 'n': case 'N':
//                    knights.set(square);
//                    pieceVector->emplace_back(Piecetype::Knight, intToSquare(square));
//                    break;
//                case 'b': case 'B':
//                    bishops.set(square);
//                    pieceVector->emplace_back(Piecetype::Bishop, intToSquare(square));
//                    break;
//                case 'q': case 'Q':
//                    queens.set(square);
//                    pieceVector->emplace_back(Piecetype::Queen, intToSquare(square));
//                    break;
//                case 'k': case 'K':
//                    kings.set(square);
//                    pieceVector->emplace_back(Piecetype::King, intToSquare(square));
//                    break;
//                case 'p':
//                    bpawns.set(square);
//                    pieceVector->emplace_back(Piecetype::bPawn, intToSquare(square));
//                    break;
//                case 'P':
//                    wpawns.set(square);
//                    pieceVector->emplace_back(Piecetype::wPawn, intToSquare(square));
//                    break;
//                default:
//                    // TODO error handling
//                    std::cerr << "Robert fucked up" << std::endl;
//            }
//            if(isupper(ch)) {
//                white.set(square);
//            }
//            else {
//                black.set(square);
//            }
//            square++;
//        }
//
//        if (isdigit(ch)) {
//            square += ch - '0';
//        }
//
//        FENIndex++;
//    }
//
//    activePlayer = (fenString[++FENIndex] == 'w') ? Color::White : Color::Black;
//    FENIndex += 2;
//    while (fenString[FENIndex] != ' ')
//    {
//        switch(fenString[FENIndex])
//        {
//            case 'K': wKC = true; break;
//            case 'Q': wKC = true; break;
//            case 'k': bKC = true; break;
//            case 'q': bKC = true; break;
//        }
//        FENIndex++;
//    }
//
//    enPassantSquare = (fenString[++FENIndex] == '-') ? NoSquare : squareFromString(fenString.substr(FENIndex, 2));
//    FENIndex += (enPassantSquare == NoSquare) ? 0 : 2;
//
//    #ifdef DEBUG
//        logger.log("%d %d %d %d %d %d %d %d", activePlayer, halfMoveClock, fullMoveNumber, enPassantSquare, wKC, wQC, bKC, bQC);
//    #endif
//
//    FENIndex++;
//    while ( (FENIndex < fenString.length()) && (fenString[++FENIndex] != ' ') ) {
//        if (!isdigit(fenString[FENIndex])) {
//            throw "invalid FENstring\n";
//            // FIXME implement real error handling
//        }
//        halfMoveClock *= 10;
//        halfMoveClock += fenString[FENIndex] - '0';
//    }
//
//    while ( (++FENIndex < fenString.length())) {
//        if (!isdigit(fenString[FENIndex])) {
//            throw "invalid FENstring\n";
//            // FIXME implement real error handling
//        }
//        fullMoveNumber *= 10;
//        fullMoveNumber += fenString[FENIndex] - '0';
//    }
//    _assert(checkBoardConsistency());
//    #ifdef DEBUG
//        logger.log("%d %d %d %d %d %d %d %d", activePlayer, halfMoveClock, fullMoveNumber, enPassantSquare, wKC, wQC, bKC, bQC);
//    #endif
//}


//bool Board::checkBoardConsistency(bool quiet/* = true */) const {
//    #ifdef DEBUG
//    // First fill the counts map from wPieces and bPieces
//    std::map<Piecetype, size_t> counts;
//
//    std::vector<Piece> allPieces;
//    allPieces.insert(allPieces.end(), wPieces.begin(), wPieces.end());
//    allPieces.insert(allPieces.end(), bPieces.begin(), bPieces.end());
//
//    for (const auto& piece : allPieces) {
//        Bitboard* bitboard = m_pieceTypeBitboards.at(piece.type);
//
//        std::string logstring = "[ASSERT] piece location: " + piecetypeStringMap.at(piece.type) + " at " + squareStringMap.at(piece.square);
//        if (!quiet) logger.log(logstring);
//        _assert(bitboard->test(piece.square));
//
//        if (counts.count(piece.type) == 0) {
//            counts[piece.type] = 1;
//        } else {
//            counts[piece.type]++;
//        }
//    }
//
//    // Check material counts
//    for (const auto& [pieceType, cnt] : counts) {
//        Bitboard* bitboard = m_pieceTypeBitboards.at(pieceType);
//        std::string logmsg = "[ASSERT] material count: " + piecetypeStringMap.at(pieceType) + " = " + std::to_string(cnt);
//        if (!quiet) logger.log(logmsg);
//
//        bool expr = (bitboard->count() == cnt);
//        _assert(expr);
//    }
//
//    // Check if any bitboards have overlapping bits set
//    if (!quiet) logger.log("[ASSERT] no overlap global: white, black");
//    _assert((white & black) == 0);
//    for (auto it1 = m_bitboardNames.begin(); it1 != m_bitboardNames.end(); ++it1) {
//        auto& [pName1, bitboard1] = *it1;
//
//        for (auto it2 = std::next(it1); it2 != m_bitboardNames.end(); ++it2) {
//            auto& [pName2, bitboard2] = *it2;
//
//            std::string logstring = "[ASSERT] no overlap: pType1: " + pName1 + ", pType2: " + pName2;
//            if (!quiet) logger.log(logstring);
//            _assert((*bitboard1 & *bitboard2) == 0);
//        }
//    }
//    #endif
//    return true;
//}

//bool Board::inCheck() const
//{
//    // TODO: implement
//    return false;
//}


//Piece Board::findPiece(Square toSquare)
//{
//    // _assert(checkBoardConsistency());
//    std::vector<Piece> *pieces = (activePlayer == Color::White) ? &bPieces : &wPieces;
//    for (auto& piece : *pieces) {
//        if (piece.square == toSquare) {
//            return piece;
//        }
//    }
//    return Piece();
//}

//const char* getPieceChar(Piecetype pType, Color color) {
//    switch (pType) {
//        case Piecetype::wPawn: return "P";
//        case Piecetype::bPawn: return "p";
//        case Piecetype::Rook: return (color == Color::White) ? "R" : "r";
//        case Piecetype::Knight: return (color == Color::White) ? "N" : "n";
//        case Piecetype::Bishop: return (color == Color::White) ? "B" : "b";
//        case Piecetype::Queen: return (color == Color::White) ? "Q" : "q";
//        case Piecetype::King: return (color == Color::White) ? "K" : "k";
//        default: return "?";
//    }
//}

//Square squareFromString(const std::string& algebraic) {
//    for (const auto& entry : squareStringMap) {
//        if (entry.second == algebraic) {
//            return entry.first;
//        }
//    }
//    return NoSquare;  // Return NoSquare if the string is not found
//}

void Board::logBoard() const
{
    std::ostringstream os;
    char board[BOARD_SIZE];

    for (char & i : board)
        i = *".";

    for (const auto& squarePiecetypePair : whitePieceMap) {
        board[squarePiecetypePair.first] = whitePiecetypeCharMap.at(squarePiecetypePair.second);
    }
    for (const auto& piece : blackPieceMap) {
        board[piece.first] = blackPiecetypeCharMap.at(piece.second);
    }

    for (int rank = 0; rank < BOARD_DIMENSIONS; rank++) {
        os << BOARD_DIMENSIONS - rank << " ";
        for (int file = 0; file < BOARD_DIMENSIONS; file++) {
            int index = rank * BOARD_DIMENSIONS + (file);
            os << " " << board[index];
        }
        os  << std::endl;
    }
    os << "\n   a b c d e f g h\n";
    os << "enPassantSquare: " << squareStringMap.at(enPassantSquare) << std::endl;
    logger.log(os);
}