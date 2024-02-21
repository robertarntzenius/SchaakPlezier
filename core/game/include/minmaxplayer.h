#pragma once

#include "player.h"

class MinMaxPlayer : public Player {
public:
    MinMaxPlayer() = default;

    [[nodiscard]] int decideOnMove (Board boardCopy, const std::vector<Move> &moves) override {
        const int maxDepth = 6;
        double bestEval;
        int bestMove = 0;

        for (const auto &move : moves) {
            std::array<bool, NrCastlingRights> copyCastlingRights = boardCopy.getCastlingRights();
            Square copyEnPassantSquare = boardCopy.getEnPassantSquare();
            int copyFiftyMoveCounter = boardCopy.getfiftyMoveCounter();

            boardCopy.doMove(move);
            const double moveEval = minMaxSearch(boardCopy, maxDepth - 1);
            boardCopy.undoMove(move, copyCastlingRights, copyEnPassantSquare, copyFiftyMoveCounter);

//            if (moveEval)
        }

        return 0;
    }

    [[nodiscard]] double minMaxSearch(const Board &board, int depth) {
        if (depth <= 0) {
            return evaluate(board);
        }



        // FIXME not actually this, but smth like this
        return minMaxSearch(board, depth - 1);
    }


//    // Recursive helper function for move application
//    void countLeafNodes(Board& board, int depth, u_int64_t &move_count) {
//        if (depth <= 0) {
//            move_count++;
//            return;
//        }
//
//        std::vector<Move> legal_moves;
//        board.getPossibleMoves(legal_moves);
//
//        for (const auto& move : legal_moves) {
//            std::array<bool, NrCastlingRights> copyCastlingRights = board.getCastlingRights();
//            Square copyEnPassantSquare = board.getEnPassantSquare();
//            int copyFiftyMoveCounter = board.getfiftyMoveCounter();
//
//            board.doMove(move);
//            countLeafNodes(board, depth - 1, move_count);
//            board.undoMove(move, copyCastlingRights, copyEnPassantSquare, copyFiftyMoveCounter);
//        }
//    }



    [[nodiscard]] double evaluate(const Board &board) {
        return 0.0;
    }

    PlayerType getPlayerType() override { return MinMax; };
};