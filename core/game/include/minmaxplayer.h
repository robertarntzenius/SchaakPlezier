#pragma once

#include "player.h"

class MinMaxPlayer : public Player {
public:
    MinMaxPlayer(PlayerSettings settings) : maxDepth(settings.MinMax_Depth), evaluated(0) {}

    [[nodiscard]] Move decideOnMove(Board board, const std::vector<Move> &moves) override {
        int sideFactor;

        switch (board.getActivePlayer()) {
            case White: sideFactor = 1.0; break;
            case Black: sideFactor = -1.0; break;
            default: throw std::invalid_argument("Invalid color: " + std::to_string(board.getActivePlayer()));
        }
        double bestEval = MIN_EVAL * sideFactor;

        size_t bestMove = 0;

        for (size_t moveIndex = 0; moveIndex <  moves.size(); moveIndex++) {

            board.doMove(moves[moveIndex]);
            const double currentEval = minMaxSearch(board, maxDepth - 1, -sideFactor);
            board.undoMove();

            if ( (currentEval * sideFactor) > bestEval) {
                bestEval = currentEval * sideFactor;
                bestMove = moveIndex;
            }
        }
        // std::cout << "evaluated: " << evaluated << std::endl;
        return moves[bestMove];
    }

    [[nodiscard]] double minMaxSearch(Board &board, int depth, int sideFactor) {
        if (depth <= 0) { // or checkmate?
            return evaluate(board);
        }
        std::vector<Move> moves;
        board.getPossibleMoves(moves);

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
            const double currentEval = minMaxSearch(board, depth - 1, -sideFactor);
            board.undoMove();

            if ( (currentEval * sideFactor) > bestEval) {
                bestEval = currentEval * sideFactor;
            }
        }
        return bestEval;
    }

    [[nodiscard]] double evaluate(const Board &board) {
        double eval = 0.0;
        evaluated++;
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

    [[nodiscard]] PlayerType getPlayerType() override { return MinMax; };

    [[nodiscard]] json getSettings() override {
        json settings =
        {
            {"playerType", playerTypeStringMap.at(MinMax)},
            {"maxDepth",  maxDepth}
        };
        return settings;
    }

private:
    int maxDepth;
    int evaluated;
};
