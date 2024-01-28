#include "board.h"

/* public */

Board::Board(const char *FENString)
    : logger(ChessLogger::getInstance()),
      piecetypeBitboards(),
      colorBitboards(),
      pieceMaps(),
      activePlayer(White),
      wKC(false), wQC(false), bKC(false), bQC(false),
      enPassantSquare(NoSquare),
      halfMoveClock(0),
      fullMoveNumber(0)
{
    #if defined(DEBUG)
        logger.setLogLevel(LEVEL_DEBUG);
    #elif defined(VERBOSE)
        // TODO implement
        logger.setLogLevel(LEVEL_VERBOSE);
    #endif
    logger.essential("New Board created!");

    InitializeFromFEN(FENString);
}

void Board::getPossibleMoves(std::vector<Move> &moveVector) const {
    logger.logHeader("getPossibleMoves");
    logBoard();

    // NOTE: loop over all player pieces here and call methods for (pseudo-)legality from switch case
    //       this removes the need for a lot of methods and loops over all pieces just to find ones of
    //       a specific type.

    // for every piece
    for (const auto &squarePiecetypePair: pieceMaps[activePlayer]) {
        const Square &fromSquare = squarePiecetypePair.first;
        const Piecetype &type = squarePiecetypePair.second;

        switch (type) {
            case Pawn:
                generatePawnMoves(moveVector, fromSquare);
                break;
            case Knight:
                generateKnightMoves(moveVector, fromSquare);
                break;
           case Bishop:
                generateSliderMoves(moveVector, fromSquare, Bishop);
                break;
            case Rook:
                generateSliderMoves(moveVector, fromSquare, Rook);
                break;
           case Queen:
                generateSliderMoves(moveVector, fromSquare, Queen);
                break;
           case King:
                generateKingMoves(moveVector, fromSquare);
                break;
            default:
                // Not implemented / throw
                break;
        }
    }
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
//    // TODO: dont forget to lose castling rights when you move/capture a rook@starting square
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

bool Board::inCheck(Color player) const
{
    // TODO: implement
    return false;
}

void Board::logBitboards() const {
    for (int colorInt = 0; colorInt < NrColors; ++colorInt) {
        const auto color = static_cast<Color>(colorInt);
        logger.verbose(colorStringMap.at(color).c_str(), colorBitboards[color]);
    }

    for (int piecetypeInt = 0; piecetypeInt < NrPiecetypes; ++piecetypeInt) {
        const auto type = static_cast<Piecetype>(piecetypeInt);
        logger.verbose(piecetypeStringMap.at(type).c_str(), piecetypeBitboards[type]);
    }
}

/* private */

void Board::InitializeFromFEN(const char *FENString)
{
    std::string boardString;
    char activeColorChar = 0;
    std::string castlingRightsString;
    std::string enPassantSquareString;

    std::istringstream iss(FENString);
    iss >> boardString >> activeColorChar >> castlingRightsString
        >> enPassantSquareString >> halfMoveClock >> fullMoveNumber;

    int squareInt = 0;

    for (const char &c : boardString) {
        switch (c) {
            case '/':
                break;
            case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8':
                squareInt += c - '0';
                break;
            default:
                const Square square = intToSquare(squareInt);
                Piecetype type = charPiecetypeMap.at(c);
                piecetypeBitboards[type].set(square);

                if (isupper(c) != 0) {
                    colorBitboards[White].set(square);
                    pieceMaps[White][square] = type;
                } else {
                    colorBitboards[Black].set(square);
                    pieceMaps[Black][square] = type;
                }

                squareInt++;
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

    #ifdef DEBUG
        checkBoardConsistency();
    #endif
}

void Board::checkBoardConsistency() const
{
    _assert(pieceMaps[White].size() == colorBitboards[White].count());
    _assert(pieceMaps[Black].size() == colorBitboards[Black].count());

    for (const auto &squarePiecetypePair : pieceMaps[White]) {
        const Square square = squarePiecetypePair.first;
        const Piecetype type = squarePiecetypePair.second;

        _assert(colorBitboards[White].test(square));
        _assert(piecetypeBitboards[type].test(square));
    }

    Bitboard noOverlapBoard = Bitboard();
    for (const auto &bitboard : colorBitboards) {
        _assert((bitboard & noOverlapBoard).empty());
        noOverlapBoard = noOverlapBoard | bitboard;
    }

    noOverlapBoard.reset();
    for (const auto &bitboard : piecetypeBitboards) {
        _assert((bitboard & noOverlapBoard).empty());
        noOverlapBoard = noOverlapBoard | bitboard;
    }
}

Bitboard Board::getAttackedMask(Color player) const {
    Bitboard attacks;
    for (const auto piece: pieceMaps[player]) {
        const Square &fromSquare = piece.first;
        const Piecetype &pieceType = piece.second;
        if (pieceType == Queen || pieceType == Rook || pieceType == Bishop) {
            attacks = attacks | getAttacksFromSlider(fromSquare, pieceType);
        }
    }

    attacks = attacks | MaskGeneration::computeKnightScopeFromBitboard(piecetypeBitboards[Knight] & colorBitboards[player]);
    attacks = attacks | MaskGeneration::computePawnAttacksFromBitboard(piecetypeBitboards[Pawn] & colorBitboards[player], player);
    attacks = attacks | MaskGeneration::computeKingScopeFromBitboard(piecetypeBitboards[King] & colorBitboards[player]);
    return attacks;
}

Bitboard Board::getAttacksFromSlider(Square fromSquare, Piecetype pieceType) const {
    const Bitboard occupied = colorBitboards[Black] | colorBitboards[White];
    uint8_t firstDirection, lastDirection;
    Bitboard scope;

    switch (pieceType) {
        case Queen:
            firstDirection = FirstOrthogonal;
            lastDirection = LastDiagonal;
        break;

        case Bishop:
            firstDirection = FirstDiagonal;
            lastDirection = LastDiagonal;
        break;

        case Rook:
            firstDirection = FirstOrthogonal;
            lastDirection = LastOrthogonal;
        break;
        
        default:
            throw std::invalid_argument("Invalid Piecetype, generateSliderMoves() should be called with a slider");
        break;
    }

    // Slider attacks
    for (uint8_t direction = firstDirection; direction <= lastDirection; direction++) {
        Bitboard directionalScope = directionalLookUp[direction][fromSquare];
        Square nearestPieceLocation = NoSquare;

        switch (direction) {
            case North:
            case West:
            case NorthEast:
            case NorthWest:
                nearestPieceLocation = (directionalScope & occupied).getLowestSetBit();
                scope = scope | directionalScope.resetUpperBits(nearestPieceLocation);
                break;
            case South:
            case East:
            case SouthEast:
            case SouthWest:
                nearestPieceLocation = (directionalScope & occupied).getHighestSetBit();
                scope = scope | directionalScope.resetLowerBits(nearestPieceLocation);
                break;
            default:
                throw std::invalid_argument("Direction of Slider should be diagonal or orthogonal. Invalid direction received.");
        }
        scope.set(nearestPieceLocation); // .resetLowerBits() is including
    }
    return scope;
}

void Board::logBoard() const {
    std::ostringstream os;
    std::string board(BOARD_SIZE, '.');

    for (const auto& squarePiecetypePair : pieceMaps[White]) {
        const Square square = squarePiecetypePair.first;
        const Piecetype type = squarePiecetypePair.second;

        board[square] = whitePiecetypeCharMap.at(type);
    }

    for (const auto& squarePiecetypePair : pieceMaps[Black]) {
        const Square square = squarePiecetypePair.first;
        const Piecetype type = squarePiecetypePair.second;

        board[square] = blackPiecetypeCharMap.at(type);
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

    os << "\nactivePlayer: " << activePlayer << std::endl;
    os << "enPassantSquare: " << enPassantSquare << std::endl;
    os << "wKC: " << wKC << ", wQC: " << wQC << ", bKC: " << bKC << ", bQC: " <<  bQC << std::endl;
    os << "halfMoveClock: " << halfMoveClock << ", fullMoveNumber: " << fullMoveNumber;
    logger.essential(os.str());
}