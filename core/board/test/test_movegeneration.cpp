#include <iostream>
#include <chrono>
#include "board.h"
#include "log.h"

constexpr const char *testFEN5 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";

std::unordered_map<std::string, std::pair<const char *, std::vector<u_int64_t>>> positionToLeafNodesMap = {
    {"defaultFEN", 
        {defaultStartingFEN, {1, 20, 400, 8902, 197281, 4865609}} }, // , 197281, 4865609 , 119060324
    {"testFEN5", 
        {testFEN5, {1, 48, 2039, 97862, 4085603, 193690690}} } //, 4085603, 193690690, 8031647685
};

// Recursive helper function for move application
void countLeafNodes(Board& board, int depth, u_int64_t &move_count) {
    if (depth <= 0) {
        move_count++;
        return;
    }

    std::vector<Move> legal_moves;
    BoardState copyState;

    board.getPossibleMoves(legal_moves, copyState);

    for (const auto& move : legal_moves) {
        board.doMove(move);
        countLeafNodes(board, depth - 1, move_count);
        board.undoMove(move, copyState);
    }
}

uint64_t test_MoveGenerationMoveApplicationPerformance(const char *FEN, const std::string& name, int depth) {
    if (depth <= 0) {
        return 1;
    }

    Board board = Board(FEN, "PerformanceTest_" + name + ".log");
    board.setLogLevel(LEVEL_ESSENTIAL);

    ChessLogger &testLogger = ChessLogger::getInstance("PerformanceTest_" + name + ".log");
    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<Move> moves;
    BoardState copyState;

    board.getPossibleMoves(moves, copyState);
    u_int64_t leaf_nodes = 0;

    testLogger.essential("\n\n");
    testLogger.essential(name, depth);
    testLogger.essential(FEN);
    board.logBoard(LEVEL_ESSENTIAL);

    for (auto move : moves) {
        board.doMove(move);

        u_int64_t move_count = 0;
        countLeafNodes(board, depth - 1, move_count);

        leaf_nodes += move_count;
        board.undoMove(move, copyState);
        
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
    return leaf_nodes;
}

int main() {
    for (const auto& test_entry : positionToLeafNodesMap) {
        const std::string &name = test_entry.first;
        const char* FEN = test_entry.second.first;
        const std::vector<uint64_t> &expectedLeafNodes = test_entry.second.second;

        int max_depth = expectedLeafNodes.size();
        
        for (int depth = 1; depth < max_depth; depth++) {
            uint64_t leaf_nodes = test_MoveGenerationMoveApplicationPerformance(FEN, name, depth);
            std::cout << leaf_nodes << " " << depth << " " << expectedLeafNodes[depth] << std::endl;
             _assert(leaf_nodes == expectedLeafNodes[depth]);
        }
    }
    return 0;
}
