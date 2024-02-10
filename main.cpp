#include <iostream>

#include "game.h"

// NOTE: Might want to move this to test file
constexpr const char *testFEN1 = "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2";
constexpr const char *testFEN2 = "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/8/PPPBBPPP/R3K2R b KQ - 3 2";
constexpr const char *testFEN3 = "rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9";
constexpr const char *testFEN4 = "rnbqkbnr/p1p1pppp/1p6/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3";
constexpr const char *gameOverFEN = "k7/2KP4/8/8/8/8/8/8 b - - 20 10";


int main(int argc, char *argv[]) {
    const char *selectedFEN = defaultStartingFEN;

    // TODO use getopt obviously
    if (argc >= 2) {
        selectedFEN = argv[1];
    }

    Game game(selectedFEN);

    try {
        game.start();

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}