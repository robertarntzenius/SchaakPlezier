from typing import Optional

from PyQt5.QtCore import QEventLoop
from wrappers import Board, Move, PlayerType, makePlayer

from schaak_plezier.interface.player import IPlayer
from schaak_plezier.widgets.chessboard_view import ChessboardView


class Player(IPlayer):
    def __init__(self, player_type: PlayerType):
        if player_type.name.lower() == "human":
            raise ValueError("Dont make human players with this class. Only cpp players")
        self.player_type = player_type
        self._player = makePlayer(player_type.name)

    def decideOnMove(self, board: Board) -> Move:
        return self._player.decideOnMove(board, board.getPossibleMoves())


class HumanPlayer(IPlayer):
    chessboard_view: ChessboardView
    clicked_move: Optional[Move]
    player_type = PlayerType.Human

    def __init__(self, chessboard_view: ChessboardView):
        super().__init__()
        self.chessboard_view = chessboard_view
        chessboard_view.validMoveClicked.connect(self.handle_valid_move_clicked)
        self.clicked_move = None

    def decideOnMove(self, *args, **kwargs) -> Move:
        # This func gets called by app at the start of our turn
        # Wait for the signal emitted by chessboard view: self.validMoveClicked.emit(move) (e.g. human decided on a move)
        # use the value of the signal to make the move.
        local_event_loop = QEventLoop()
        self.chessboard_view.validMoveClicked.connect(local_event_loop.quit)
        local_event_loop.exec_()

        if self.clicked_move is None:
            raise RuntimeError("Exited event loop unexpectedly. Cliked move cannot be None.")

        return self.clicked_move

    def handle_valid_move_clicked(self, move: Move) -> None:
        self.clicked_move = move
