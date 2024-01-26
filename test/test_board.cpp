#include "board.h"
#include <cassert>

constexpr const char *testFEN1 = "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2";
constexpr const char *testFEN2 = "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/8/PPPBBPPP/R3K2R b KQ - 3 2";
constexpr const char *testFEN3 = "rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9";
constexpr const char *testFEN4 = "rnbqkbnr/p1p1pppp/1p6/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3";


// FIXME use testing database when movegen is implemented
void test_board_default_getPossibleMoves() {
    ChessLogger& logger = ChessLogger::getInstance();

    Board board = Board();

    std::vector<Move> moves;
    board.getPossibleMoves(moves);
    for (const auto& move : moves) {
            logger.log(move);
    }
    assert(moves.size() == 20); // 16 pawnmoves, 4 knightmoves
}

void test_board_TestFENS_getPossibleMoves() {
    ChessLogger& logger = ChessLogger::getInstance();
    std::pair<const char *, int> testFENs[] = {
            {testFEN1, 36},
            {testFEN2, 37},
            {testFEN3, 33},
            {testFEN4, 26}
    };
    for (const auto &testEntry : testFENs) {
        const char *FENstring = testEntry.first;
        const std::size_t correctNrMoves = testEntry.second;
        Board board = Board(FENstring);

        std::vector<Move> moves;
        board.getPossibleMoves(moves);
        for (const auto& move : moves) {
            logger.log(move);
        }
        std::cerr << "expected: " << correctNrMoves << "\n";
        std::cerr << "actual: " << moves.size() << "\n";
        assert(moves.size() == correctNrMoves);
    }
}


int main() {
    test_board_default_getPossibleMoves();
    test_board_TestFENS_getPossibleMoves();
    // doMove
    // inCheck
    // logBoard
    // logBitboards
    return 0;
}