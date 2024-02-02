#include "board.h"

/* public */

Board::Board(const char *FENString, const std::string &logFile)
    : logger(ChessLogger::getInstance(logFile)),
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

// FIXME should be const, maybe move incheck call & doMove somewhere
void Board::getPossibleMoves(std::vector<Move> &moveVector) {
    logger.logHeader("getPossibleMoves");
    logBoard(LEVEL_DEBUG);

    // NOTE: loop over all player pieces here and call methods for (pseudo-)legality from switch case
    //       this removes the need for a lot of methods and loops over all pieces just to find ones of
    //       a specific type.

    // for every piece
    std::vector<Move> psuedoLegalMoves;

    for (const auto &squarePiecetypePair: pieceMaps[activePlayer]) {
        const Square &fromSquare = squarePiecetypePair.first;
        const Piecetype &type = squarePiecetypePair.second;

        switch (type) {
            case Pawn:
                generatePawnMoves(psuedoLegalMoves, fromSquare);
                break;
            case Knight:
                generateKnightMoves(psuedoLegalMoves, fromSquare);
                break;
           case Bishop:
                generateSliderMoves(psuedoLegalMoves, fromSquare, Bishop);
                break;
            case Rook:
                generateSliderMoves(psuedoLegalMoves, fromSquare, Rook);
                break;
           case Queen:
                generateSliderMoves(psuedoLegalMoves, fromSquare, Queen);
                break;
           case King:
                generateKingMoves(psuedoLegalMoves, fromSquare);
                break;
            default:
                // Not implemented / throw
                break;
        }
    }

    for (const auto& move : psuedoLegalMoves) {
        std::array<bool, NrCastlingRights> copyCastlingRights = getCastlingRights();
        Square copyEnPassantSquare = enPassantSquare;

        doMove(move);
        if (!inCheck(~activePlayer)) {
            moveVector.emplace_back(std::move(move));
        }        
        undoMove(move, copyCastlingRights, copyEnPassantSquare);
        // checkBoardConsistency();
    }
}

void Board::movePiece(Color player, Piecetype pieceType, Square fromSquare, Square toSquare) {
    if (toSquare != NoSquare) {
        piecetypeBitboards[pieceType].set(toSquare);
        pieceMaps[player][toSquare] = pieceType;
        colorBitboards[player].set(toSquare);
    }

    if (fromSquare != NoSquare) {
        piecetypeBitboards[pieceType].set(fromSquare, false);
        colorBitboards[player].set(fromSquare, false);
        pieceMaps[player].erase(fromSquare);
    }
}

std::array<bool, 4> Board::getCastlingRights() const {
    return {wKC, wQC, bKC, bQC};
}

void Board::setCastlingRights(std::array<bool, NrCastlingRights> &newCastlingRights) {
    wKC = newCastlingRights[wKingside];
    wQC = newCastlingRights[wQueenside];
    bKC = newCastlingRights[bKingside];
    bQC = newCastlingRights[bQueenside];
}

Square Board::getEnPassantSquare() const {
    return enPassantSquare;
}

void Board::setLogLevel(LogLevel logLevel)
{
    logger.setLogLevel(logLevel);
}

void Board::setEnPassantSquare(Square newEnpassantSquare) {
    enPassantSquare = newEnpassantSquare;
}


void Board::doMove(const Move &move) {   
    if (move.isCapture) {
        movePiece(~activePlayer, move.capturePiece, move.captureSquare, NoSquare);
    }

    movePiece(activePlayer, move.playerPiece, move.fromSquare, move.targetSquare);

    if (move.isPromotion) {
        piecetypeBitboards[Pawn].set(move.targetSquare, false);
        piecetypeBitboards[move.promotionPiece].set(move.targetSquare);
        pieceMaps[activePlayer][move.targetSquare] = move.promotionPiece;
    }

    if (move.isCastling) {
        switch (move.targetSquare) {
        case g1:
            movePiece(activePlayer, Rook, h1, f1);
            break;
        case c1:
            movePiece(activePlayer, Rook, a1, d1);
            break;
        case g8:
            movePiece(activePlayer, Rook, h8, f8);
            break;
        case c8:
            movePiece(activePlayer, Rook, a8, d8);
            break;
        default:
            throw std::invalid_argument("Non valid Castle move");
        }
    }
    
    auto removeCastlingRights = [this](Square square) {
        switch(square) {
            case a1: wQC = false; break;
            case h1: wKC = false; break;
            case a8: bQC = false; break;
            case h8: bKC = false; break;
            case e1: wQC = false; wKC = false; break;
            case e8: bQC = false; bKC = false; break;
            default: break; 
        }
    };

    removeCastlingRights(move.fromSquare);
    removeCastlingRights(move.targetSquare);

    halfMoveClock++;
    if ((halfMoveClock % 2) == 0) {
        fullMoveNumber++;
    }
    enPassantSquare = move.newEnPassant;
    activePlayer = ~activePlayer;
}

void Board::undoMove(const Move &move, std::array<bool, NrCastlingRights> copyCastlingRights, Square copyEnPassantSquare) {
    activePlayer = ~activePlayer;

    setCastlingRights(copyCastlingRights);
    setEnPassantSquare(copyEnPassantSquare);

    if ((halfMoveClock % 2) == 0) {
        fullMoveNumber--;
    }
    halfMoveClock--;

    if (move.isCastling) {
        switch (move.targetSquare) {
        case g1:
            movePiece(activePlayer, Rook, f1, h1);
            break;
        case c1:
            movePiece(activePlayer, Rook, d1, a1);
            break;
        case g8:
            movePiece(activePlayer, Rook, f8, h8);
            break;
        case c8:
            movePiece(activePlayer, Rook, d8, a8);
            break;
        default:
            throw std::invalid_argument("Non valid Castle move");
        }
    }

    if (move.isPromotion) {
        piecetypeBitboards[Pawn].set(move.targetSquare, true);
        piecetypeBitboards[move.promotionPiece].set(move.targetSquare, false);
        pieceMaps[activePlayer][move.targetSquare] = Pawn;
    }

    movePiece(activePlayer, move.playerPiece, move.targetSquare, move.fromSquare);

    if (move.isCapture) {
        movePiece(~activePlayer, move.capturePiece, NoSquare, move.captureSquare);
    }
}

bool Board::inCheck(Color player) const
{
    Bitboard opponentAttacks = getPlayerAttackMask(~player);
    Bitboard playerKing = piecetypeBitboards[King] & colorBitboards[player];
    return (!(opponentAttacks & playerKing).empty());
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

Bitboard Board::getPlayerAttackMask(Color player) const {
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

void Board::logBoard(LogLevel logLevel) const {
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
    
    switch (logLevel) {
        case LEVEL_ESSENTIAL:   logger.essential(os.str()); break;
        case LEVEL_DEBUG:       logger.debug(os.str());     break;
        case LEVEL_VERBOSE:     logger.verbose(os.str());   break;
        default: break;
        }
    }