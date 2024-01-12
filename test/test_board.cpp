#include "board.h"
#include <cassert>


constexpr const char *testFEN1 = "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2";
constexpr const char *testFEN2 = "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/8/PPPBBPPP/R3K2R b KQ - 3 2";
constexpr const char *testFEN3 = "rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9";
constexpr const char *testFEN4 = "rnbqkbnr/p1p1pppp/1p6/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3";


// FIXME use testing database when movegen is implemented
void test_board_default_getPossibleMoves() {
    Board board = Board();

    std::vector<Move> moves;
    board.getPossibleMoves(moves);

    assert(moves.size() == 20); // 16 pawnmoves, 4 knightmoves
}

void test_board_TestFENS_getPossibleMoves() {
    const char *testFENs[] = {testFEN1, testFEN2, testFEN3, testFEN4};
    for (const auto &FENstring : testFENs) {
        Board board = Board(FENstring);

        std::vector<Move> moves;
        board.getPossibleMoves(moves);
        // assert(moves.size() == 20); // TODO refactor
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