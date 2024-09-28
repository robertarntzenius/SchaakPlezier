#include "game.h"

#include <utility>
#include "log.h"


Game::Game(std::unique_ptr<Player> whitePlayer, std::unique_ptr<Player> blackPlayer, const char *FENString)
    : playerFactory(),
      players{std::move(whitePlayer), std::move(blackPlayer)},
      logger(ChessLogger::getInstance()),
      FENString(FENString),
      board(Board(FENString))
{
}

Game::Game(const std::string &whitePlayer, const std::string &blackPlayer, const char *FENString)
    : playerFactory(),
      players{playerFactory.makePlayer(whitePlayer), playerFactory.makePlayer(blackPlayer)},
      logger(ChessLogger::getInstance()),
      FENString(FENString),
      board(Board(FENString))
{
}

GameResult Game::start(bool print)
{
    std::vector<Move> moves;

    const size_t RESERVE = 64;
    moves.reserve(RESERVE);

    Move move;
    if (print) {
        logger.verbose("Starting game with players:");
        logger.verbose(players[White]->getPlayerType());
        logger.verbose(players[Black]->getPlayerType());

        // Clear screen
        std::cout << "\033[2J\033[H";

        // Opening messages
        std::cout << "Starting new game. " << board.getActivePlayer() << " to play.\n"
                << "white: " << players[White]->getPlayerType() << ", black: " << players[Black]->getPlayerType() << "\n"
                << "Type \"quit\" to end the game. Enter moves by typing a square your "
                << "piece occupies followed by the square you want to move it to. (e2e4)\n\n"
                << "Enjoy!\n";
    }

    while (true) {
    	if (print) {
            // Print board
            std::cout << board;
        }

        board.getPossibleMoves(moves);
        if (board.getGameResult(moves.empty()) != NOT_OVER) break;

        // Player interaction
        Move move = players[board.getActivePlayer()]->decideOnMove(board, moves);

	board.doMove(move);

        if (print) {
            logger.verbose(move);
            // Flush screen
            std::cout << "\033[2J\033[H";
        }
    }

    // Game over
    if (print) {
        std::cout << board;
        std::cout << board.getGameResult(moves.empty()) << std::endl;
        logger.essential(board.getGameResult(moves.empty()));
    }
    return board.getGameResult(moves.empty());
}

void Game::setFEN(std::string newFENString)
{
    FENString = std::move(newFENString);
}

void Game::resetBoard()
{
    board.clearBoard();
    board.initFromFEN(FENString.c_str());
}

void Game::setPlayer(Color color, const std::string &player)
{
    PlayerSettings settings;
    players[color] = playerFactory.makePlayer(player, settings);
}

json Game::getPlayerSettings(Color color)
{
    return players[color]->getSettings();
}
