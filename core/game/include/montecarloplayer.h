#include "player.h"
#include <omp.h>


class MonteCarloPlayer : public Player {
    public:
        explicit MonteCarloPlayer (int nrIterations = 50)
            : Player(),
              nrIterations(nrIterations),
              randomDevice(),
              gen(randomDevice())
        {
        }

        [[nodiscard]] size_t decideOnMove(Board board, const std::vector<Move> &moves) override {
            size_t bestMove = 0;
            size_t currentEval = 0;
            size_t bestEval = 0;
            size_t moveIndex = 0;

            for (; moveIndex < moves.size(); moveIndex++) {
                currentEval = 0;
                #pragma omp parallel for
                for (size_t i = 0; i < nrIterations; i++) {
                    GameResult result = simulateGame(board);
                    
                    if ((board.getActivePlayer() == White && result == WHITE_WIN_BY_CHECKMATE) ||
                        (board.getActivePlayer() == Black && result == BLACK_WIN_BY_CHECKMATE)) {
                        #pragma omp atomic
                        currentEval++;
                    }                                    
                }
                
                if (currentEval > bestEval) {
                    bestEval = currentEval;
                    bestMove = moveIndex;
                }            
            }
            
            return bestMove;
        }

    PlayerType getPlayerType() override { return MonteCarlo; };

private:
        [[nodiscard]] GameResult simulateGame(Board board) {
            std::vector<Move> moves;

            board.getPossibleMoves(moves);
            
            int depth = 0;
            static const int limit = 50;
                    
            while (board.getGameResult(moves.empty()) == NOT_OVER) {
                if (depth >= limit) {
                    return (evaluate(board) >= 0)? WHITE_WIN_BY_CHECKMATE : BLACK_WIN_BY_CHECKMATE;
                }

                std::uniform_int_distribution<> dist(0, moves.size() - 1);
                int randomMove = dist(gen);
                board.doMove(moves[randomMove]);
                board.getPossibleMoves(moves);
                depth++;
            }
            
            return board.getGameResult(moves.empty());
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

        size_t nrIterations;

        std::random_device randomDevice;
        std::mt19937 gen;

};
