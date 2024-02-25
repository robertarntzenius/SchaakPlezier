#include "game.h"
#include "log.h"


Game::Game(std::unique_ptr<Player> whitePlayer, std::unique_ptr<Player> blackPlayer, const char *FENString)
    : whitePlayer(std::move(whitePlayer)),
      blackPlayer(std::move(blackPlayer)),
      logger(ChessLogger::getInstance()),
      board(Board(FENString))
{
}

void Game::start()
{
    std::vector<Move> moves;

    const size_t RESERVE = 64;
    moves.reserve(RESERVE);

    Move move;

    logger.debug("Starting game with players:");
    logger.debug(whitePlayer->getPlayerType());
    logger.debug(blackPlayer->getPlayerType());

    // Clear screen
    std::cout << "\033[2J\033[H";

    // Opening messages
    std::cout << "Starting new game. " << board.getActivePlayer() << " to play.\n"
              << "white: " << whitePlayer->getPlayerType() << ", black: " << blackPlayer->getPlayerType() << "\n"
              << "Type \"quit\" to end the game. Enter moves by typing a square your "
              << "piece occupies followed by the square you want to move it to. (e2e4)\n\n"
              << "Enjoy!\n";

    BoardState copyState;
    while (true) {
        board.getPossibleMoves(moves, copyState);
        logger.essential(board.getGameResult(moves.size() == 0));

        // Print board
        std::cout << board;

        

        // Player interaction
        if (board.getActivePlayer() == White) {
            size_t player1Choice = whitePlayer->decideOnMove(board, moves, copyState);
            if (player1Choice == SIZE_MAX) {
                // Quit
                return;
            }
            move = moves[player1Choice];
        }
        else {
            size_t player2Choice = blackPlayer->decideOnMove(board, moves, copyState);
            if (player2Choice == SIZE_MAX) {
                // Quit
                return;
            }
            move = moves[player2Choice];
        }

        board.doMove(move);

        logger.debug(move);

        std::cout << "\033[2J\033[H";
    }
}

