#include "game.h"
#include "log.h"


Game::Game(std::unique_ptr<Player> player1, std::unique_ptr<Player> player2, const char *FENString)
    : player1(std::move(player1)),
      player2(std::move(player2)),
      logger(ChessLogger::getInstance()),
      board(Board(FENString))
{
}

void Game::start()
{
    std::vector<Move> moves;
    moves.reserve(64);

    Move move;

    logger.debug("Starting game with players:");
    logger.debug(player1->getPlayerType());
    logger.debug(player2->getPlayerType());

    // Clear screen
    std::cout << "\033[2J\033[H";

    // Opening messages
    std::cout << "Starting new game. " << board.getActivePlayer() << " to play.\n"
              << "white: " << player1->getPlayerType() << ", black: " << player2->getPlayerType() << "\n"
              << "Type \"quit\" to end the game. Enter moves by typing a square your "
              << "piece occupies followed by the square you want to move it to. (e2e4)\n\n"
              << "Enjoy!\n";

    while (true) {
        board.getPossibleMoves(moves);

        // Print board
        std::cout << board;

        // Check for (stale)mate
        if (moves.empty()) {
            // Game over
            if (board.inCheck()) {
                // Win for not-active player
                std::cout << ~board.getActivePlayer() << " player won the game! \n";
                logger.debug("Game finished;", ~board.getActivePlayer(), "won!");
                return;
            }

            // Stalemate
            std::cout << "Stalemate! \n";
            logger.debug("Stalemate");
            return;
        }
        
        // Insufficient material
        if (board.checkInsufficientMaterial()) {
            std::cout << "Game finished; draw by insufficient material!\n";
            logger.debug("Game finished; draw by insufficient material!");
            return;
        }

        // TODO implement
        if (board.checkFiftyMoveRule()) {
            std::cout << "Game finished; draw by 50 move rule!\n";
            logger.debug("Game finished; draw by 50 move rule!");
            return;
        }

        // TODO implement
        if (board.checkThreeFoldRepetition()) {
            std::cout << "Game finished; draw by three fold repetition!\n";
            logger.debug("Game finished; draw by three fold repetition!");
            return;
        }

        // Player interaction
        if (board.getActivePlayer() == White) {
            int player1Choice = player1->decideOnMove(board, moves);
            if (player1Choice == -1) {
                // Quit
                return;
            }
            move = moves[player1Choice];
        }
        else {
            int player2Choice = player2->decideOnMove(board, moves);
            if (player2Choice == -1) {
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

