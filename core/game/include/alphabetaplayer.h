#pragma once

#include "player.h"
#include <cfloat>

class AlphaBetaPlayer : public Player {
public:
    explicit AlphaBetaPlayer(PlayerSettings settings) : maxDepth(settings.AlphaBeta_Depth), positionsEvaluated(0),  positionsQuiescence(0) {}

    [[nodiscard]] Move decideOnMove(Board board, const std::vector<Move> &moves) override {
        double alpha = 2 * MIN_EVAL; // DBL_MIN;
        double beta = 2 * MAX_EVAL; //DBL_MAX;
        double currentEval = 0;
        size_t bestMoveIndex = 0;
        bool isMaximize = (board.getActivePlayer() == White);

        for (size_t i = 0; i < moves.size(); i++) {
            board.doMove(moves[i]);
            if (isMaximize) {
                currentEval = alphaBetaMin(board, maxDepth - 1, alpha, beta);
                if (currentEval > alpha) {

                    // std::cout << "alpha: " << alpha << "\ncurrentEval: "<<  currentEval << "\nmove: " << moves[i] << std::endl;

                    alpha = currentEval;
                    bestMoveIndex = i;
                }
            }
            else {
                currentEval = alphaBetaMax(board, maxDepth - 1, alpha, beta);
                if (currentEval < beta) {
                    // std::cout << "beta: " << beta << "\ncurrentEval: "<<  currentEval << "\nmove: " << moves[i] << std::endl;
                    beta = currentEval;
                    bestMoveIndex = i;
                }
            }
            board.undoMove();

        }

        std::cout << "Positions evaluated: " << positionsEvaluated << std::endl;
        std::cout << "Positions Quiescence'd: " << positionsQuiescence << std::endl;
        return moves[bestMoveIndex];
    }

    /*
    alpha: minimum assured score for maximizing player
    beta:  maximum assured score for minimizing player
    */
    [[nodiscard]] double alphaBetaMax(Board &board, int depth, double alpha, double beta) {
        bool noLegalMoves = true;
        std::vector<Move> moves;
        moves.reserve(64);

        if (depth > 0) {
            board.getPossibleMoves(moves);
            noLegalMoves = moves.empty();
        }
        else {
            positionsQuiescence++;

            double standPat = evaluate(board);
            if (standPat >= beta)
                return beta;
            if( standPat > alpha )
                alpha = standPat;
            board.getLoudMoves(moves, noLegalMoves);

            // TODO better move ordering
            auto PrioSort = [](const Move& first, const Move& second){
                double firstPriority = -pieceValues[first.playerPiece];
                double secondPriority = -pieceValues[second.playerPiece];

                if (first.isCapture) firstPriority += pieceValues[first.capturePiece];
                if (second.isCapture) secondPriority += pieceValues[second.capturePiece];

                return firstPriority >= secondPriority;
            };

            std::sort(moves.begin(), moves.end(), PrioSort);

        }

        switch (board.getGameResult(noLegalMoves)) {
            case NOT_OVER:
                break;

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

        for (auto move : moves) {
            board.doMove(move);
            const double currentEval = alphaBetaMin(board, depth - 1, alpha, beta);
            board.undoMove();

            if ( currentEval >= beta) {
                return beta;
            }
            if (currentEval > alpha) {
                alpha = currentEval;
            }
        }
        return alpha;
    }

    [[nodiscard]] double alphaBetaMin(Board &board, int depth, double alpha, double beta) {
        bool noLegalMoves = true;
        std::vector<Move> moves;
        moves.reserve(64);

        if (depth > 0) {
            board.getPossibleMoves(moves);
            noLegalMoves = moves.empty();
        }
        else {
            positionsQuiescence++;

            double standPat = evaluate(board);
            if (standPat <= alpha)
                return alpha;
            if( standPat < beta )
                beta = standPat;
            board.getLoudMoves(moves, noLegalMoves);

            // TODO better move ordering
            auto PrioSort = [](const Move& first, const Move& second){
                double firstPriority = -pieceValues[first.playerPiece];
                double secondPriority = -pieceValues[second.playerPiece];

                if (first.isCapture) firstPriority += pieceValues[first.capturePiece];
                if (second.isCapture) secondPriority += pieceValues[second.capturePiece];

                return firstPriority >= secondPriority;
            };

            std::sort(moves.begin(), moves.end(), PrioSort);
        }

        switch (board.getGameResult(moves.empty())) {
            case NOT_OVER:
                break;

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

        for (auto move : moves) {
            board.doMove(move);
            const double currentEval = alphaBetaMax(board, depth - 1, alpha, beta);
            board.undoMove();

            if ( currentEval <= alpha) {
                return alpha;
            }
            if (currentEval < beta) {
                beta = currentEval;
            }
        }
        return beta;
    }

    [[nodiscard]] double evaluate(const Board &board) {
        double eval = 0.0;
        positionsEvaluated++;

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

    [[nodiscard]] PlayerType getPlayerType() override { return AlphaBeta; };

    [[nodiscard]] json getSettings() override {
        json settings =
        {
            {"playerType", playerTypeStringMap.at(AlphaBeta)},
            {"maxDepth",  maxDepth}
        };
        return settings;
    }

private:
    int maxDepth;
    int positionsEvaluated;
    int positionsQuiescence;
};
