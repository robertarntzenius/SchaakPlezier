#include "player.h"


class MonteCarloPlayer : public Player {
    public:
        MonteCarloPlayer (int nrIterations = 100) : Player(),
                                                   nrIterations(nrIterations)
        {
            std::srand(static_cast<unsigned int>(std::time(nullptr)));
        }

        [[nodiscard]] size_t decideOnMove(Board board, const std::vector<Move> &moves, const BoardState &copyState) override {
            size_t bestMove = 0;
            size_t currentEval = 0;
            size_t bestEval = 0;
            size_t moveIndex = 0;

            for (; moveIndex < moves.size(); moveIndex++) {
                currentEval = 0;

                for (int i = 0; i < nrIterations; i++) {
                    GameResult result = simulateGame(board);
                    
                    if ((board.getActivePlayer() == White && result == WHITE_WIN_BY_CHECKMATE) ||
                        (board.getActivePlayer() == Black && result == BLACK_WIN_BY_CHECKMATE)) {
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

    private:
        [[nodiscard]] GameResult simulateGame(Board board) {
            std::vector<Move> moves;
            BoardState copyState;

            board.getPossibleMoves(moves, copyState);
            
            int depth = 0;
            static const int limit = 50;
                    
            while (board.getGameResult(moves.size() == 0) == NOT_OVER) {
                if (depth >= limit) {
                    return (evaluate(board) >= 0)? WHITE_WIN_BY_CHECKMATE : BLACK_WIN_BY_CHECKMATE;
                }
            
                int randomMove = std::rand() % moves.size();
                board.doMove(moves[randomMove]);
                board.getPossibleMoves(moves, copyState);
                depth++;
            }
            
            return board.getGameResult(moves.size() == 0);
        }

        [[nodiscard]] double evaluate(const Board &board) {
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
   
    PlayerType getPlayerType() override { return MonteCarlo; };

    const int nrIterations;        
};
