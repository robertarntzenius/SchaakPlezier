#pragma once

#include "player.h"

#define MAX_EVAL 100.0
#define MIN_EVAL -100.0


class MinMaxPlayer : public Player {
public:
    MinMaxPlayer() = default;

    [[nodiscard]] size_t decideOnMove (Board boardCopy, const std::vector<Move> &moves, const BoardState &copyState) override {
        int sideFactor;
        switch (copyState.activePlayer) {
            case White: sideFactor = 1; break;
            case Black: sideFactor = -1; break;
            default: throw std::invalid_argument("Invalid color: " + std::to_string(copyState.activePlayer));
        }
        double bestEval = MIN_EVAL * sideFactor;

        const int maxDepth = 6;
        size_t bestMove = 0;

        for (size_t moveIndex = 0; moveIndex <  moves.size(); moveIndex++) {                        
            boardCopy.doMove(moves[moveIndex]);
            const double currentEval = minMaxSearch(boardCopy, maxDepth - 1, -sideFactor);
            boardCopy.undoMove(moves[moveIndex], copyState);
            
            if (currentEval > (bestEval * sideFactor)) {
                bestEval = currentEval;
                bestMove = moveIndex;
            }
        }

        return bestMove;
    }

    [[nodiscard]] double minMaxSearch(Board &board, int depth, int sideFactor) {
        if (depth <= 0) { // or checkmate?
            return evaluate(board);
        }

        BoardState copyState;
        std::vector<Move> moves;
        board.getPossibleMoves(moves, copyState);
        double bestEval = MIN_EVAL * sideFactor;

        for (size_t currentMove = 0; currentMove < moves.size(); currentMove++) {
            board.doMove(moves[currentMove]);
            double currentEval = minMaxSearch(board, depth - 1, -sideFactor);
            board.undoMove(moves[currentMove], copyState);
            
            if (currentEval > (bestEval * sideFactor)) {
                bestEval = currentEval;
            }
        }
        return bestEval;
    }

    [[nodiscard]] double evaluate(const Board &board) {
        return 0.0;
    }

    PlayerType getPlayerType() override { return MinMax; };
};