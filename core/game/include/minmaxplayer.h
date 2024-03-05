#pragma once

#include "player.h"


class MinMaxPlayer : public Player {
public:

    MinMaxPlayer() = default;

    [[nodiscard]] size_t decideOnMove(Board board, const std::vector<Move> &moves, const BoardState &copyState) override {
        int sideFactor;
        switch (copyState.activePlayer) {
            case White: sideFactor = 1.0; break;
            case Black: sideFactor = -1.0; break;
            default: throw std::invalid_argument("Invalid color: " + std::to_string(copyState.activePlayer));
        }
        double bestEval = MIN_EVAL * sideFactor;

        const int maxDepth = 2;
        size_t bestMove = 0;

        for (size_t moveIndex = 0; moveIndex <  moves.size(); moveIndex++) {                        
            board.doMove(moves[moveIndex]);
            const double currentEval = minMaxSearch(board, maxDepth - 1, -sideFactor);
            board.undoMove(moves[moveIndex], copyState);
            
            if ( (currentEval * sideFactor) > bestEval) {
                bestEval = currentEval * sideFactor;
                bestMove = moveIndex;
            }
        }

        return bestMove;
    }

    [[nodiscard]] double minMaxSearch(Board &board, int depth, int sideFactor) {
        if (depth <= 0) { // or checkmate?
            return evaluate(board);
        }
        BoardState copyState{};
        std::vector<Move> moves;
        board.getPossibleMoves(moves, copyState);
        
        switch (board.getGameResult(moves.empty())) {
            case NOT_OVER: break;

            case WHITE_WIN_BY_CHECKMATE:
            case WHITE_WIN_BY_TIME_OUT:
            case WHITE_WIN_BY_FORFEIT:
                return MAX_EVAL + depth;

            case BLACK_WIN_BY_CHECKMATE:
            case BLACK_WIN_BY_TIME_OUT: 
            case BLACK_WIN_BY_FORFEIT: 
                return MIN_EVAL - depth;

            case DRAW_BY_STALEMATE: 
            case DRAW_BY_INSUFFICIENT_MATERIAL: 
            case DRAW_BY_REPETITION: 
            case DRAW_BY_50_MOVES:
                return 0.0;

            default:
                throw std::invalid_argument("Invalid Game Result: " + std::to_string(board.getGameResult(moves.empty())));
            }

        double bestEval = MIN_EVAL * sideFactor;

        for (auto move : moves) {
            board.doMove(move);
            double currentEval = minMaxSearch(board, depth - 1, -sideFactor);
            board.undoMove(move, copyState);
            
            if ( (currentEval * sideFactor) > bestEval) {
                bestEval = currentEval * sideFactor;
            }
        }
        return bestEval;
    }

    [[nodiscard]] static double evaluate(const Board &board) {
        double eval = 0.0;

        // White
        for (const auto &entry : board.getPieceMap(White)) {
            const Piecetype &pieceType = entry.second;
            eval += pieceValues[pieceType];
        }

        // Black
        for (const auto &entry : board.getPieceMap(Black)) {
            const Piecetype &pieceType = entry.second;
            eval -= pieceValues[pieceType];
        }
        return eval;
    }

    PlayerType getPlayerType() override { return MinMax; };
};
