from abc import abstractmethod
from typing import Optional, Protocol

from PyQt5.QtCore import QEventLoop
from wrappers import Board, Move, PlayerType, makePlayer

from schaak_plezier.widgets.chessboard_view import ChessboardView


class IPlayer(Protocol):
    @abstractmethod
    def getPlayerType(self) -> PlayerType: ...

    @abstractmethod
    def decideOnMove(self, board: Board) -> Move: ...


class CppPlayer(IPlayer):
    def __init__(self, player_type: PlayerType):
        if player_type == PlayerType.Human:
            raise ValueError("Dont make human players with this class. Only cpp players")
        self.player_type = player_type
        self._player = makePlayer(player_type.name)

    def getPlayerType(self) -> PlayerType:
        return self.player_type

    def decideOnMove(self, board: Board) -> Move:
        return self._player.decideOnMove(board, board.getPossibleMoves())


class HumanPlayer(IPlayer):
    chessboard_view: ChessboardView
    clicked_move: Optional[Move] = None
    player_type = PlayerType.Human

    def __init__(self, chessboard_view: ChessboardView):
        super().__init__()
        self.chessboard_view = chessboard_view
        chessboard_view.validMoveClicked.connect(lambda move: self.handle_valid_move_clicked(move))
        self._event_loop = QEventLoop()

    def getPlayerType(self) -> PlayerType:
        return self.player_type

    def decideOnMove(self, board: Optional[Board] = None) -> Move:
        # This func gets called by app at the start of our turn
        # Wait for the signal emitted by chessboard view: self.validMoveClicked.emit(move) (e.g. human decided on a move)
        # use the value of the signal to make the move.
        self.clicked_move = None

        self._event_loop.exec_()

        if self.clicked_move is None:
            raise RuntimeError("Exited event loop unexpectedly. Cliked move cannot be None.")

        return self.clicked_move

    def handle_valid_move_clicked(self, move: Move) -> None:
        self.clicked_move = move
        self._event_loop.quit()
