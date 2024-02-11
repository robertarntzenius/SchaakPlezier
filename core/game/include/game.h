#pragma once

#include "board.h"
#include "log.h"
#include "player.h"

#include <algorithm>

class Game {
    public:
        Game(std::unique_ptr<Player> whitePlayer, std::unique_ptr<Player> blackPlayer, const char *FENString = defaultStartingFEN);
        ~Game() = default;

        void start();

    private:
        std::unique_ptr<Player> whitePlayer;
        std::unique_ptr<Player> blackPlayer;

        ChessLogger& logger;
        Board board;
};