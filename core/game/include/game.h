#pragma once

#include "board.h"
#include "log.h"
#include "player.h"

#include <algorithm>

class Game {
    public:
        Game(std::unique_ptr<Player> player1, std::unique_ptr<Player> player2, const char *FENString = defaultStartingFEN);
        ~Game() = default;

        void start();

    private:
        std::unique_ptr<Player> player1;
        std::unique_ptr<Player> player2;

        ChessLogger& logger;
        Board board;
};