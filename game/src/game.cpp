#include "game.h"
#include "log.h"

Game::Game(const char *FENString) 
    : logger(ChessLogger::getInstance()),
      board(Board(FENString))
{
}

void Game::start()
{
    std::vector<Move> moves;
    moves.reserve(64);

    HumanPlayer player1;
    RandomPlayer player2;
    Move move;

    logger.debug("Starting game!");

    // Clear screen
    std::cout << "\033[2J\033[H";

    // Opening messages
    std::cout << "Starting new game. " << board.getActivePlayer() << " to play.\n"
              << "Type \"quit\" to end the game. Enter moves by typing a square your "
              << "piece occupies followed by the square you want to move it to. (e2e4)\n\n"
              << "Enjoy!\n";

    while (true) {
        board.getPossibleMoves(moves);

        // Print board
        std::cout << board;

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
        
        // Player interaction
        if (board.getActivePlayer() == White) {
            int player1Choice = player1.decideOnMove(board, moves);
            if (player1Choice == -1) {
                // Quit
                return;
            }
            move = moves[player1Choice];
        }
        else {
            int player2Choice = player2.decideOnMove(board, moves);
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
