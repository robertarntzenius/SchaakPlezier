#pragma once

#include "player.h"
#include <float.h>

class AlphaBetaPlayer : public Player {
public:
    AlphaBetaPlayer(PlayerSettings settings) : maxDepth(settings.AlphaBeta_Depth), positionsEvaluated(0) {}

    [[nodiscard]] size_t decideOnMove(Board board, const std::vector<Move> &moves) override {
        double alpha = 2*MIN_EVAL;// DBL_MAX;
        double beta = 2*MAX_EVAL; //DBL_MAX;
        size_t bestMoveIndex = 2000;

        static bool a = false;
        if (!a) {
            for (const auto& move: moves) {
                std::cout << move << ", ";
            }
            std::cout << std::endl;
            a = true;
        }

        for (size_t i = 0; i < moves.size(); i++) {
            board.doMove(moves[i]);
            double currentEval = -alphaBetaSearch(board, maxDepth - 1, -beta, -alpha);
            board.undoMove();

            if (currentEval > alpha) {
                std::cout << "alpha: " << alpha << "\ncurrentEval: "<<  currentEval << "move: " << moves[i] << std::endl;
                alpha = currentEval;
                bestMoveIndex = i;
            }
        }
        std::cout << "Positions evaluated: " << positionsEvaluated << std::endl;
        
        if (bestMoveIndex == 2000 ) {
            std::cout << "??" << std::endl;
            return 0;
        }
        return bestMoveIndex;        
    }

    /*
    alpha: minimum assured score for maximizing player
    beta:  maximum assured score for minimizing player
    */
    [[nodiscard]] double alphaBetaSearch(Board &board, int depth, double alpha, double beta) {
        if (depth <= 0) { // or checkmate?
            return evaluate(board);
            // return quiesce(board, alpha, beta);
        }
        
        std::vector<Move> moves;
        moves.reserve(64);
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

        for (auto move : moves) {
            board.doMove(move);
            const double currentEval = -alphaBetaSearch(board, depth - 1, -beta, -alpha);
            board.undoMove();
            
            if ( currentEval >= beta) {
                // std::cout << "pruned at depth " << depth << std::endl;
                return beta;
            }
            if (currentEval > alpha) {
                alpha = currentEval;
            }
        }
        return alpha;
    }

    // TODO add limit for depth or nodes
    [[nodiscard]] double quiesce(Board &board, double alpha, double beta) {
    //     int Quiesce( int alpha, int beta ) {
    //     int stand_pat = Evaluate();
    //     if( stand_pat >= beta )
    //         return beta;
    //     if( alpha < stand_pat )
    //         alpha = stand_pat;

    //     until( every_capture_has_been_examined )  {
    //         MakeCapture();
    //         score = -Quiesce( -beta, -alpha );
    //         TakeBackMove();

    //         if( score >= beta )
    //             return beta;
    //         if( score > alpha )
    //         alpha = score;
    //     }
    //     return alpha;
    // }
        double currentEval = evaluate(board);
        if( currentEval >= beta )
            return beta;
        if( alpha < currentEval )
            alpha = currentEval;

        std::vector<Move> loudMoves;
        loudMoves.reserve(64);
        bool noLegalMoves = false;

        board.getLoudMoves(loudMoves, noLegalMoves);
        
        switch (board.getGameResult(noLegalMoves)) {
            case NOT_OVER: break;

            case WHITE_WIN_BY_CHECKMATE:
            case WHITE_WIN_BY_TIME_OUT:
            case WHITE_WIN_BY_FORFEIT:
            case BLACK_WIN_BY_CHECKMATE:
            case BLACK_WIN_BY_TIME_OUT: 
            case BLACK_WIN_BY_FORFEIT: 
                std::cout << "mate found: " << board.getGameResult(noLegalMoves) << std::endl;
                return MAX_EVAL;

            case DRAW_BY_STALEMATE: 
            case DRAW_BY_INSUFFICIENT_MATERIAL: 
            case DRAW_BY_REPETITION: 
            case DRAW_BY_50_MOVES:
                return 0.0;

            default:
                throw std::invalid_argument("Invalid Game Result: " + std::to_string(board.getGameResult(noLegalMoves)));
            }

        if (loudMoves.size() == 0) { // Quiet position
            return evaluate(board);
        }

        for (auto move : loudMoves) {
            board.doMove(move);
            const double currentEval = -quiesce(board, -beta, -alpha);
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

    [[nodiscard]] double evaluate(const Board &board) {
        double eval = 0.0;
        positionsEvaluated++;

        // White
        for (const auto &entry : board.getPieceMap(board.getActivePlayer())) {
            const Piecetype &pieceType = entry.second;
            eval += pieceValues[pieceType];
        }

        // Black
        for (const auto &entry : board.getPieceMap(~board.getActivePlayer())) {
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
};
