// #include "board.h"


// void Board::generatePawnMoves(std::vector<std::unique_ptr<Move>> &moveVector) const {
// #ifdef DEBUG
//     logger.logHeader("generatePawnMoves()");
// #endif

//     const Bitboard occupied = (colorBitboards[White] | colorBitboards[Black]);
//     const Bitboard finalRank = (activePlayer == White) ? MaskGeneration::computeRankMask(Rank8) : MaskGeneration::computeRankMask(Rank1); // TODO fix robert

//     // for every piece
//     for (const auto &squarePiecetypePair: pieceMaps[activePlayer]) {
//         const Square fromSquare = squarePiecetypePair.first;
//         const Piecetype type = squarePiecetypePair.second;

//         // for every pawn
//         if (type != Pawn) {
//             continue;
//         }

//         // Pawn pushes
//         const Bitboard pushes = pawnPushLookUp[activePlayer][fromSquare];
//         std::vector<Square> toSquares = pushes.getIndices();

//         for (const auto &toSquare : toSquares) {
//             if (occupied.test(toSquare)) {
//                 continue;
//             }
            
//             // Double push (check in between)
//             if (abs(toSquare - fromSquare) == 2 * BOARD_DIMENSIONS) {
//                 const Square newEnPassantSquare = intToSquare((toSquare + fromSquare) / 2);

//                 if (occupied.test(newEnPassantSquare)) {
//                     continue;
//                 }

//                 const DoublePawnMove move = {{fromSquare, toSquare, Move::DoublePawn}, newEnPassantSquare};
//                 moveVector.emplace_back(std::make_unique<DoublePawnMove>(move));
//             } 

//             // Single push
//             else {
//                 const Move move = {fromSquare, toSquare, Move::Basic, finalRank.test(toSquare)};
//                 moveVector.emplace_back(std::make_unique<Move>(move));
//             }
//         }

//         const Bitboard attacks = pawnAttackLookUp[activePlayer][fromSquare];

//         toSquares = (attacks & colorBitboards[invertColor(activePlayer)]).getIndices();
//         for (const auto &toSquare : toSquares) {
//             const CaptureMove move = {{fromSquare, toSquare, Move::Capture, finalRank.test(toSquare)}, toSquare};
//             moveVector.emplace_back(std::make_unique<CaptureMove>(move));
//         }

//         if (attacks.test(enPassantSquare)) {
            
//             const Square captureSquare = rankFileToSquare(squareToRank(fromSquare), squareToFile(enPassantSquare));
//             const CaptureMove move = {{fromSquare, enPassantSquare, Move::Capture}, captureSquare};

//             moveVector.emplace_back(std::make_unique<CaptureMove>(move));
//         }
//     }
// }