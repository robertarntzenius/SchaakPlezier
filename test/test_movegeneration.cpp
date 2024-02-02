#include <iostream>
#include <chrono>
#include "board.h"
#include "log.h"


// Recursive helper function for move application
void getLeafNodes(Board& board, int depth, u_int64_t &move_count) {
    if (depth <= 1) {
        move_count++;
        return;
    }

    std::vector<Move> legal_moves;
    board.getPossibleMoves(legal_moves);

    for (const auto& move : legal_moves) {
        board.checkBoardConsistency();
        
        std::array<bool, 4> copyCastlingRights = board.getCastlingRights();
        Square copyEnPassantSquare = board.getEnPassantSquare();
        
        board.doMove(move);
        getLeafNodes(board, depth - 1, move_count);
        board.undoMove(move, copyCastlingRights, copyEnPassantSquare);
    }
}

void performMoveApplicationPerfTest(Board &board, int depth) {
    ChessLogger &testLogger = ChessLogger::getInstance("PerformanceTest.log");
    testLogger.essential("\n");

    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<Move> moves;
    board.getPossibleMoves(moves);
    u_int64_t leaf_nodes = 0;
    u_int64_t move_count = 0;
    
    for (auto move : moves) {
        board.checkBoardConsistency();
        
        std::array<bool, 4> copyCastlingRights = board.getCastlingRights();
        Square copyEnPassantSquare = board.getEnPassantSquare();
        
        board.doMove(move);

        move_count = 0;
        getLeafNodes(board, depth, move_count);

        leaf_nodes += move_count;
        board.undoMove(move, copyCastlingRights, copyEnPassantSquare);
        
        testLogger.essential(move, move_count);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    testLogger.essential("depth:");
    testLogger.essential(depth);
    
    testLogger.essential("duration in ms:");
    testLogger.essential(duration);

    testLogger.essential("leaf nodes: ");
    testLogger.essential(leaf_nodes);
}

int main() {
    // 6 takes 40 s, 7 takes 10+ min in debug mode
    int max_depth = 5; 

    Board board = Board(defaultStartingFEN, "PerformanceTest.log");
    board.setLogLevel(LEVEL_ESSENTIAL);
    for (int i = 0; i <= max_depth; i++) {
        performMoveApplicationPerfTest(board, i);
    }
    return 0;
}
