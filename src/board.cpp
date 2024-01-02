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

    #ifdef DEBUG
        logBoard();
        logBitboards();
    #endif
}

void Board::getPossibleMoves(std::vector<std::unique_ptr<Move>> &moveVector) const {
    #ifdef DEBUG
    logger.logHeader("getPossibleMoves()");
    logBoard();
    #endif

    // FIXME: should vector contain normal ptrs or a smart ptrs for safety?

    generatePawnMoves(moveVector);
}

void Board::doMove(const Move *move) {
    // TODO: reimplement


    // This function checks the legality of a move at the end by checking
    // if the king is in check in the resulting position return -1
    // if not in check return 0;

    // This function only changes the bitboards depending on the Move it receives

    // Before changing any of the player bitboards, see if the move is a capture
    // if the move is a capture, change necessary opponent bitboards first
    // move.capture is the pType of the captured piece

    // TODO change occupied & empty and other bitboards I might've missed.
    // These arent being used now, so not very important at the moment

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
}

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

    checkBoardConsistency();
    #ifdef DEBUG
        logger.log("%d %d %d %d %d %d %d %d", activePlayer, halfMoveClock, fullMoveNumber, enPassantSquare, wKC, wQC, bKC, bQC);
    #endif
}



void Board::generatePawnMoves(std::vector<std::unique_ptr<Move>> &moveVector) const {
#ifdef DEBUG
    logger.logHeader("generatePawnMoves()");
#endif
    // TODO: reimplement

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
//#ifdef DEBUG
//            Move move(fromSquare, toSquare, playerPawnType, findPiece(toSquare));
//            logger.log("CAPTURE");
//            logger.log(move);
//#endif
//            pawnMoves.emplace_back(fromSquare, toSquare, playerPawnType, findPiece(toSquare));
//        }
//
//        // enPassantSquare
//        // cant use validAttacks because the enPassantSquare square is empty by definition
//        if ((enPassantSquare != NoSquare) && (PawnAttacks[color][fromSquare].test(enPassantSquare))) {
//#ifdef DEBUG
//            Move move(fromSquare, enPassantSquare, playerPawnType, opponentPawnType);
//            logger.log("ENPASSANT");
//            logger.log(move);
//#endif
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
//                 && (empty.test(intToSquare((toSquare+fromSquare)/2))) // if one square in front is empty
//                 && (empty.test(toSquare)) )
//            {
//#ifdef DEBUG
//                Move move(fromSquare, toSquare, playerPawnType, NoType);
//                logger.log("DOUBLE PUSH");
//                logger.log(move);
//#endif
//                pawnMoves.emplace_back(fromSquare, toSquare, playerPawnType, NoType, (intToSquare((toSquare+fromSquare)/2)));
//
//                // TODO set enpassant square or at least pass it along with the move?
//            }
//
//                // Single pushes
//            else if ( (abs(fromSquare - toSquare) == BOARD_DIMENSIONS)
//                      && empty.test(toSquare))
//            {
//#ifdef DEBUG
//                Move move(fromSquare, toSquare, playerPawnType, NoType);
//                logger.log("SINGLE PUSH");
//                logger.log(move);
//#endif
//                pawnMoves.emplace_back(fromSquare, toSquare, playerPawnType, NoType);
//            }
//        }
//    }
//    return pawnMoves;
}


void Board::checkBoardConsistency() const
{
    _assert(whitePieceMap.size() == colorBitboardMap.at(White).count());
    _assert(blackPieceMap.size() == colorBitboardMap.at(Black).count());

    for (const auto &squarePiecetypePair : whitePieceMap) {
        const Square square = squarePiecetypePair.first;
        const Piecetype type = squarePiecetypePair.second;

        _assert(colorBitboardMap.at(White).test(square));
        _assert(piecetypeBitboardMap.at(type).test(square));
    }

    Bitboard noOverlapBoard = Bitboard();
    for (const auto &colorBitboardPair : colorBitboardMap) {
        const Bitboard bitboard = colorBitboardPair.second;
        _assert((bitboard & noOverlapBoard).empty());
        noOverlapBoard = noOverlapBoard | bitboard;
    }

    noOverlapBoard.reset();
    for (const auto &piecetypeBitboardPair : piecetypeBitboardMap) {
        const Bitboard bitboard = piecetypeBitboardPair.second;
        _assert((bitboard & noOverlapBoard).empty());
        noOverlapBoard = noOverlapBoard | bitboard;
    }
}

bool Board::inCheck(Color player) const
{
    // TODO: implement
    return false;
}

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