#pragma once

#include "playerfactory.h"
#include "player.h"
#include "board.h"
#include "log.h"

#include <vector>
#include <string>


class Game {
    public:
        Game(std::unique_ptr<Player> whitePlayer, std::unique_ptr<Player> blackPlayer, const char *FENString = defaultStartingFEN);
        Game(const std::string &whitePlayer, const std::string &blackPlayer, const char *FENString = defaultStartingFEN);
        ~Game() = default;

        GameResult start(bool print = true);
        void setFEN(std::string FENString);
        void resetBoard();
        void setPlayer(Color color, const std::string &player);
        json getPlayerSettings(Color color);

    private:
        PlayerFactory playerFactory;
        std::array<std::unique_ptr<Player>, NrColors> players;

        ChessLogger& logger;
        std::string FENString;
        Board board;
};
