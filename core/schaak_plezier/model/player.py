from PyQt5.QtCore import QEventLoop
from wrappers import Board, Move, PlayerType, makePlayer

from schaak_plezier.interface.app import IApplication
from schaak_plezier.interface.game import IPlayer


class Player(IPlayer):
    def __init__(self, player_type: PlayerType):
        if player_type == "human":
            raise ValueError("Dont make human players with this class. Only cpp players")
        self.player_type = player_type
        self._player = makePlayer(player_type)

    def decide_on_move(self, board: Board) -> Move:
        return self._player.decideOnMove(board, board.getPossibleMoves())


class HumanPlayer(IPlayer):
    def __init__(self, app: IApplication):
        super().__init__()
        self.app = app
        self.player_type = "Human"
        app.gui.chessboard_view.validMoveClicked.connect(self.handle_valid_move_clicked)
        self.clicked_move = None

    def decide_on_move(self, board: Board) -> Move:
        # This func gets called by app
        # Wait for the signal emitted by chessboard view: self.validMoveClicked.emit(move) (e.g. human decided on a move)
        # use the value of the signal to make the move.
        local_event_loop = QEventLoop()
        self.app.gui.chessboard_view.validMoveClicked.connect(local_event_loop.quit)
        local_event_loop.exec_()

        return self.clicked_move

    def handle_valid_move_clicked(self, move: Move) -> None:
        self.clicked_move = move
