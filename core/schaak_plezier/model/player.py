import wrappers
from PyQt5.QtCore import QEventLoop

from schaak_plezier.interface.game import IPlayer
from schaak_plezier.interface.wrapper_types import Move, PlayerType
from schaak_plezier.model.chessboard import Chessboard


class Player(IPlayer):
    def __init__(self, player_type: str):
        if player_type.lower() == "human":
            raise ValueError(
                "Should not make Human players with this Player class. Use HumanPlayer() instead"
            )
        self.player_type = PlayerType(player_type)
        self._player = wrappers.makePlayer(player_type)

    def decide_on_move(self, board: Chessboard) -> Move:
        move = Move(self._player.decideOnMove(board._board, board._getPossibleMoves()))
        return move


class HumanPlayer(IPlayer):
    def __init__(self, chessboard_view):
        super().__init__()
        self.player_type = PlayerType("Human")
        self.chessboard_view = chessboard_view
        self.chessboard_view.validMoveClicked.connect(self.handle_valid_move_clicked)
        self.clicked_move = None

    def decide_on_move(self, board: Chessboard) -> Move:
        # This func gets called by controller
        # Wait for the signal emitted by chessboard view: self.validMoveClicked.emit(move) (e.g. human decided on a move)
        # use the value of the signal to make the move.
        local_event_loop = QEventLoop()
        self.chessboard_view.validMoveClicked.connect(local_event_loop.quit)
        local_event_loop.exec_()

        return self.clicked_move

    def handle_valid_move_clicked(self, move: Move) -> None:
        self.clicked_move = move
