#include <iostream>

#include "game.h"

// NOTE: Might want to move this to test file
constexpr const char *testFEN1 = "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2";
constexpr const char *testFEN2 = "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/8/PPPBBPPP/R3K2R b KQ - 3 2";
constexpr const char *testFEN3 = "rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9";

int main() {
    Game game(testFEN2);

    try {
//        game->start();
        game.test();

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}