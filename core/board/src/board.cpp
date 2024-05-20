#include "board.h"

/* public */
Board::Board(const char *FENString, const std::string &logFile)
    : logger(ChessLogger::getInstance(logFile)),
      boardState(defaultBoardState),
      history(),
      hashHistory(),
      piecetypeBitboards(),
      colorBitboards(),
      pieceMaps(),
      zobristPieceTable(),
      zobristCastlingTable(),
      zobristActivePlayer()
{
    #if defined(DEBUG)
        logger.setLogLevel(LEVEL_DEBUG);
    #elif defined(VERBOSE)
        logger.setLogLevel(LEVEL_VERBOSE);
    #endif
    logger.essential("New Board created!");

    initZobristTables();
    initFromFEN(FENString);
}

void Board::initFromFEN(const char *FENString)
{
    clearBoard();

    std::string boardString;
    char activeColorChar = 0;
    std::string castlingRightsString;
    std::string enPassantSquareString;

    std::istringstream iss(FENString);
    iss >> boardString >> activeColorChar >> castlingRightsString
        >> enPassantSquareString >> boardState.halfMoveClock >> boardState.fullMoveNumber;

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
                    hashPiece(White, type, square);
                } else {
                    colorBitboards[Black].set(square);
                    pieceMaps[Black][square] = type;
                    hashPiece(Black, type, square);
                }

                squareInt++;
                break;
        }
    }

    boardState.activePlayer = charColorMap.at(activeColorChar);
    if (boardState.activePlayer == Black) {
        hashActivePlayer();
    }

    for (const char &c : castlingRightsString) {
        switch(c) {
            case 'K': {hashCastlingRight(wKingside) ; boardState.wKC = true;} break;
            case 'Q': {hashCastlingRight(wQueenside); boardState.wQC = true;} break;
            case 'k': {hashCastlingRight(bKingside) ; boardState.bKC = true;} break;
            case 'q': {hashCastlingRight(bQueenside); boardState.bQC = true;} break;
            case '-': break;
            default:
                throw std::invalid_argument("Invalid char found in FEN parser during initialization of castling rights: " + std::to_string(c));
        }
    }

    boardState.enPassantSquare = stringSquareMap.at(enPassantSquareString);
    validate();
}

void Board::clearBoard() {
    for (auto &pieceMap : pieceMaps) {
        pieceMap.clear();
    }
    for (auto &bitboard : piecetypeBitboards) {
        bitboard.reset();
    }
    for (auto &bitboard : colorBitboards) {
        bitboard.reset();
    }
    history = std::stack<MoveCommand>();
    boardState = defaultBoardState;
}

void Board::getPseudoLegalMoves(std::vector<Move> &pseudoLegalMoves) const {
    logger.logHeader("getPseudoLegalMoves");
    logBoard(LEVEL_VERBOSE);
    pseudoLegalMoves.clear();

    for (const auto &squarePiecetypePair: pieceMaps[boardState.activePlayer]) {
        const Square &fromSquare = squarePiecetypePair.first;
        const Piecetype &type = squarePiecetypePair.second;

        switch (type) {
            case Pawn:
                generatePawnMoves(pseudoLegalMoves, fromSquare);
                break;
            case Knight:
                generateKnightMoves(pseudoLegalMoves, fromSquare);
                break;
           case Bishop:
                generateSliderMoves(pseudoLegalMoves, fromSquare, Bishop);
                break;
            case Rook:
                generateSliderMoves(pseudoLegalMoves, fromSquare, Rook);
                break;
           case Queen:
                generateSliderMoves(pseudoLegalMoves, fromSquare, Queen);
                break;
           case King:
                generateKingMoves(pseudoLegalMoves, fromSquare);
                break;
           default:
                // Not implemented / throw
                break;
        }
    }
}

void Board::getPossibleMoves(std::vector<Move> &moveVector) {
    logger.logHeader("getPossibleMoves");
    logBoard(LEVEL_VERBOSE);

    moveVector.clear();

    std::vector<Move> pseudoLegalMoves;
    pseudoLegalMoves.reserve(64);
    getPseudoLegalMoves(pseudoLegalMoves);
    
    for (const auto& move : pseudoLegalMoves) {
        doMove(move);
        if (!inCheck(~boardState.activePlayer)) {
            moveVector.emplace_back(move);
        }
        undoMove();
    }
}

void Board::getLoudMoves(std::vector<Move> &moveVector, bool &noLegalMoves) {
    logger.logHeader("getLoudMoves");
    logBoard(LEVEL_VERBOSE);

    moveVector.clear();
    std::vector<Move> pseudoLegalMoves;
    pseudoLegalMoves.reserve(64);
    getPseudoLegalMoves(pseudoLegalMoves);
    noLegalMoves = true;
    for (const auto& move : pseudoLegalMoves) {
        doMove(move);
        if (!inCheck(~boardState.activePlayer)) {
            noLegalMoves = false;
            if (move.isCapture || inCheck(boardState.activePlayer)) {
                moveVector.emplace_back(move);
            }
        }
        undoMove();
    }
}

GameResult Board::getGameResult(bool noLegalMoves) const {
    if (noLegalMoves) {
        if (inCheck()) {
            switch(~getActivePlayer()) {
                case White: return WHITE_WIN_BY_CHECKMATE; break;
                case Black: return BLACK_WIN_BY_CHECKMATE; break;
                default: throw std::invalid_argument("Invalid color: " + std::to_string(~getActivePlayer()));
            }
        }
        return DRAW_BY_STALEMATE;
    }
    
    // Insufficient material
    if (checkInsufficientMaterial()) {
        return DRAW_BY_INSUFFICIENT_MATERIAL;
    }

    if (checkFiftyMoveRule()) {
        return DRAW_BY_50_MOVES;
    }

    // TODO implement
    if (checkThreeFoldRepetition()) {
        return DRAW_BY_REPETITION;
    }
    return NOT_OVER;
}

Color Board::getActivePlayer() const {
    return boardState.activePlayer;
}

const BoardState Board::getBoardState() const
{
    return boardState;
}

const std::unordered_map<Square, Piecetype> Board::getPieceMap(Color color) const {
    return pieceMaps[color];
}

const std::array<Bitboard, NrPiecetypes> Board::getPiecetypeBitboards() const {
    return piecetypeBitboards;
}

const std::array<Bitboard, NrColors> Board::getColorBitboards() const {
    return colorBitboards;
}

const std::stack<MoveCommand> Board::getHistory() const {
    return history;
}

void Board::setBoardState(const BoardState &copyState) {
    boardState = copyState;
}

void Board::getPiecetypeBitboards(const std::array<Bitboard, NrPiecetypes> &copyPiecetypeBitboards) {
    piecetypeBitboards = copyPiecetypeBitboards; 
}

void Board::getColorBitboards(const std::array<Bitboard, NrColors> &copyColorBitboards ) {
    colorBitboards = copyColorBitboards;
}

void Board::setPieceMaps(const std::array<std::unordered_map<Square, Piecetype>, NrColors> &copyMaps) {
    pieceMaps = copyMaps;
}

void Board::hashPiece(Color player, Piecetype pieceType, Square square) {
    boardState.hash ^= zobristPieceTable[square][player][pieceType];
}

void Board::hashCastlingRight(CastlingSide side) {
    boardState.hash ^= zobristCastlingTable[side];
}

void Board::hashActivePlayer() {
    boardState.hash ^= zobristActivePlayer;
}

void Board::removeCastlingRights(Square square) {
    switch(square) {
        case a1: if (boardState.wQC) { hashCastlingRight(wQueenside); boardState.wQC = false; } break;
        case h1: if (boardState.wKC) { hashCastlingRight(wKingside);  boardState.wKC = false; } break;
        case a8: if (boardState.bQC) { hashCastlingRight(bQueenside); boardState.bQC = false; } break;
        case h8: if (boardState.bKC) { hashCastlingRight(bKingside);  boardState.bKC = false; } break;
        case e1:
            if (boardState.wQC) { hashCastlingRight(wQueenside); boardState.wQC = false; }
            if (boardState.wKC) { hashCastlingRight(wKingside);  boardState.wKC = false; } 
            break;
        case e8:
            if (boardState.bKC) { hashCastlingRight(bKingside);  boardState.bKC = false; }
            if (boardState.bQC) { hashCastlingRight(bQueenside); boardState.bQC = false; }
            break;
        default:
            break;
    }
}

void Board::movePiece(Color player, Piecetype pieceType, Square fromSquare, Square toSquare, bool updateHash) {
    if (toSquare != NoSquare) {
        piecetypeBitboards[pieceType].set(toSquare);
        pieceMaps[player][toSquare] = pieceType;
        colorBitboards[player].set(toSquare);
        if (updateHash) {
            hashPiece(player, pieceType, toSquare);
        }
    }

    if (fromSquare != NoSquare) {
        piecetypeBitboards[pieceType].set(fromSquare, false);
        colorBitboards[player].set(fromSquare, false);
        pieceMaps[player].erase(fromSquare);
        if (updateHash) {
            hashPiece(player, pieceType, fromSquare);
        }
    }
}

void Board::doMove(const Move &move) {
    history.emplace(MoveCommand(move, boardState));

    if (move.isCapture) {
        movePiece(~boardState.activePlayer, move.capturePiece, move.captureSquare, NoSquare); // capturePiece out@captureSquare
        boardState.halfMoveClock = 0;
    }
    if (move.playerPiece == Pawn) {
        boardState.halfMoveClock = 0;
    }

    movePiece(boardState.activePlayer, move.playerPiece, move.fromSquare, move.targetSquare); // playerPiece out@fromSquare & in@targetSquare

    if (move.isPromotion) {
        piecetypeBitboards[Pawn].set(move.targetSquare, false);
        piecetypeBitboards[move.promotionPiece].set(move.targetSquare);
        pieceMaps[boardState.activePlayer][move.targetSquare] = move.promotionPiece;

        hashPiece(boardState.activePlayer, Pawn, move.targetSquare); // Pawn out@targetSquare 
        hashPiece(boardState.activePlayer, move.promotionPiece, move.targetSquare); // promotionPiece in@targetSquare
    }

    if (move.isCastling) {
        switch (move.targetSquare) {
        case g1:
            movePiece(boardState.activePlayer, Rook, h1, f1);
            break;
        case c1:
            movePiece(boardState.activePlayer, Rook, a1, d1);
            break;
        case g8:
            movePiece(boardState.activePlayer, Rook, h8, f8);
            break;
        case c8:
            movePiece(boardState.activePlayer, Rook, a8, d8);
            break;
        default:
            throw std::invalid_argument("Invalid Castle move");
        }
    }
    
    removeCastlingRights(move.fromSquare);
    removeCastlingRights(move.targetSquare);

    boardState.halfMoveClock++;

    if (boardState.activePlayer == White) {
        boardState.fullMoveNumber++;
    }
    boardState.enPassantSquare = move.newEnPassant;
    boardState.activePlayer = ~boardState.activePlayer;
    hashActivePlayer();
}

void Board::undoMove() {
    MoveCommand moveCommand = history.top();
    Move move = moveCommand.move;

    boardState = moveCommand.beforeState;

    if (move.isCastling) {
        switch (move.targetSquare) {
        case g1:
            movePiece(boardState.activePlayer, Rook, f1, h1, false);
            break;
        case c1:
            movePiece(boardState.activePlayer, Rook, d1, a1, false);
            break;
        case g8:
            movePiece(boardState.activePlayer, Rook, f8, h8, false);
            break;
        case c8:
            movePiece(boardState.activePlayer, Rook, d8, a8, false);
            break;
        default:
            throw std::invalid_argument("Non valid Castle move");
        }
    }

    if (move.isPromotion) {
        piecetypeBitboards[Pawn].set(move.targetSquare, true);
        piecetypeBitboards[move.promotionPiece].set(move.targetSquare, false);
        pieceMaps[boardState.activePlayer][move.targetSquare] = Pawn;
    }

    movePiece(boardState.activePlayer, move.playerPiece, move.targetSquare, move.fromSquare, false);

    if (move.isCapture) {
        movePiece(~boardState.activePlayer, move.capturePiece, NoSquare, move.captureSquare, false);
    }
    history.pop();
}

bool Board::inCheck(Color player) const
{
    Bitboard opponentAttacks = getPlayerAttackMask(~player);
    Bitboard playerKing = piecetypeBitboards[King] & colorBitboards[player];
    return (!(opponentAttacks & playerKing).empty());
}

bool Board::inCheck() const
{
    Bitboard opponentAttacks = getPlayerAttackMask(~boardState.activePlayer);
    Bitboard playerKing = piecetypeBitboards[King] & colorBitboards[boardState.activePlayer];
    return (!(opponentAttacks & playerKing).empty());
}

uint64_t Board::computeHashFromScratch()
{
    uint64_t hashValue = 0;

    for (int colorIndex = 0; colorIndex < NrColors; colorIndex++) {
        for (const auto &piece : pieceMaps[colorIndex]) {
            const auto &square = piece.first;
            const auto &type = piece.second;
            hashValue ^= zobristPieceTable[square][colorIndex][type];
        }
    }

    hashValue = (boardState.wKC)? hashValue ^ zobristCastlingTable[wKingside]  : hashValue;
    hashValue = (boardState.wQC)? hashValue ^ zobristCastlingTable[wQueenside] : hashValue;
    hashValue = (boardState.bKC)? hashValue ^ zobristCastlingTable[bKingside]  : hashValue;
    hashValue = (boardState.bQC)? hashValue ^ zobristCastlingTable[bQueenside] : hashValue;

    hashValue = (boardState.activePlayer == Black)? hashValue ^ zobristActivePlayer : hashValue;

    return hashValue;
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

std::ostream &operator<<(std::ostream &os, const Board &board) {
    std::string boardString(BOARD_SIZE, '.');

    for (const auto& squarePiecetypePair : board.pieceMaps[White]) {
        const Square square = squarePiecetypePair.first;
        const Piecetype type = squarePiecetypePair.second;

        boardString[square] = whitePiecetypeCharMap.at(type);
    }

    for (const auto& squarePiecetypePair : board.pieceMaps[Black]) {
        const Square square = squarePiecetypePair.first;
        const Piecetype type = squarePiecetypePair.second;

        boardString[square] = blackPiecetypeCharMap.at(type);
    }

    for (int rank = 0; rank < BOARD_DIMENSIONS; rank++) {
        os << BOARD_DIMENSIONS - rank << " ";
        for (int file = 0; file < BOARD_DIMENSIONS; file++) {
            int index = rank * BOARD_DIMENSIONS + (file);
            os << " " << boardString[index];
        }
        os  << std::endl;
    }
    os << "\n   a b c d e f g h\n";
    
    os << "\n" << board.getBoardState() << std::endl;
    return os;
}


/* private */
void Board::initZobristTables() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> distribution;

    for (auto &square : zobristPieceTable) {
        for (auto &color : square) {
            for (auto &value : color) {
                value = distribution(gen);
            }
        }
    }

    for (auto &value : zobristCastlingTable)
    {
        value = distribution(gen);
    }

    zobristActivePlayer = distribution(gen);
}

void Board::validate() const {
    assert(pieceMaps[White].size() == colorBitboards[White].count());
    assert(pieceMaps[Black].size() == colorBitboards[Black].count());

    for (const auto &squarePiecetypePair : pieceMaps[White]) {
        const Square square = squarePiecetypePair.first;
        const Piecetype type = squarePiecetypePair.second;

        assert(colorBitboards[White].test(square));
        assert(piecetypeBitboards[type].test(square));
    }

    Bitboard noOverlapBoard = Bitboard();
    for (const auto &bitboard : colorBitboards) {
        assert((bitboard & noOverlapBoard).empty());
        noOverlapBoard = noOverlapBoard | bitboard;
    }

    noOverlapBoard.reset();
    for (const auto &bitboard : piecetypeBitboards) {
        assert((bitboard & noOverlapBoard).empty());
        noOverlapBoard = noOverlapBoard | bitboard;
    }
    
    assert((piecetypeBitboards[King] & colorBitboards[White]).count() == 1);
    assert((piecetypeBitboards[King] & colorBitboards[Black]).count() == 1);
    assert((finalRank[White] & piecetypeBitboards[Pawn]).empty());
    assert((finalRank[Black] & piecetypeBitboards[Pawn]).empty());

    if (boardState.wKC) {
        assert((piecetypeBitboards[King] & colorBitboards[White]).test(e1));
        assert((piecetypeBitboards[Rook] & colorBitboards[White]).test(h1));
    }
    if (boardState.wQC) {
        assert((piecetypeBitboards[King] & colorBitboards[White]).test(e1));
        assert((piecetypeBitboards[Rook] & colorBitboards[White]).test(a1));
    }
    if (boardState.bKC) {
        assert((piecetypeBitboards[King] & colorBitboards[Black]).test(e8));
        assert((piecetypeBitboards[Rook] & colorBitboards[Black]).test(h8));
    }
    if (boardState.bQC) {
        assert((piecetypeBitboards[King] & colorBitboards[Black]).test(e8));
        assert((piecetypeBitboards[Rook] & colorBitboards[Black]).test(a8));
    }

    // FIXME 
    // This is used to determine the validity FEN strings in board/test/test_hash
    // assert(!inCheck(~boardState.activePlayer));

    assert(boardState.halfMoveClock <= 50);
}

bool Board::checkInsufficientMaterial() const
{
    if (!piecetypeBitboards[Pawn].empty() || !piecetypeBitboards[Rook].empty() || !piecetypeBitboards[Queen].empty()) {
        return false;
    }

    // Only minor pieces left
    size_t nrKnights = piecetypeBitboards[Knight].count();
    size_t nrBishops = piecetypeBitboards[Bishop].count();
    size_t nrMinorPieces = nrKnights + nrBishops;

    // Definitely not enough material
    if (nrMinorPieces < 2) {
        return true;
    }

    // Definitely enough material
    if ((nrMinorPieces > 2) || nrKnights > 0) {
        return false;
    }

    // Do checks with 2 bishops on board
    Bitboard whiteBishops = (piecetypeBitboards[Bishop] & colorBitboards[White]);
    
    // One side has 2 Bishops
    if (whiteBishops.count() != 1) {
        return false;
    }

    // Different color bishops
    if ((piecetypeBitboards[Bishop] & darkSquares).count() == 1) {
        return false;
    }

    return true;
}

bool Board::checkFiftyMoveRule() const
{
    static constexpr int FIFTY = 50;
    return (boardState.halfMoveClock >= FIFTY);
}

bool Board::checkThreeFoldRepetition() const
{
    // TODO implement
    return false;
}

Bitboard Board::getPlayerAttackMask(Color player) const {
    Bitboard attacks;
    for (const auto &piece: pieceMaps[player]) {
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
    uint8_t firstDirection = 0;
    uint8_t lastDirection = 0;
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
    if (logger.getLogLevel() < logLevel) {
        return;
    }

    std::ostringstream os;

    os << *this << "\n";

    os << "activePlayer: " << boardState.activePlayer << std::endl;
    os << "enPassantSquare: " << boardState.enPassantSquare << std::endl;
    os << "wKC: " << boardState.wKC << ", wQC: " << boardState.wQC << ", bKC: " << boardState.bKC << ", bQC: " <<  boardState.bQC << std::endl;
    os << "halfMoveClock: " << boardState.halfMoveClock << ", fullMoveNumber: " << boardState.fullMoveNumber;
    
    switch (logLevel) {
        case LEVEL_ESSENTIAL:   logger.essential(os.str()); break;
        case LEVEL_DEBUG:       logger.debug(os.str());     break;
        case LEVEL_VERBOSE:     logger.verbose(os.str());   break;
        default: break;
        }
    }
