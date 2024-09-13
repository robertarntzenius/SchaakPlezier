from abc import abstractmethod
from typing import Protocol

from schaak_plezier.interface.observe import ObservableWidget
from schaak_plezier.model.piece import Piece
from wrappers import Board, Color, GameResult, Move, PlayerType


class IChessboard(ObservableWidget):
    _board: Board

    @abstractmethod
    def do_move(self, move: Move) -> None: ...

    @abstractmethod
    def undo_move(self) -> None: ...

    @abstractmethod
    def redo_move(self) -> None: ...

    @property
    @abstractmethod
    def history() -> list[Move]: ...

    @property
    @abstractmethod
    def possible_moves(self) -> list[Move]: ...

    @property
    @abstractmethod
    def wpieces(self) -> list[Piece]: ...

    @property
    @abstractmethod
    def bpieces(self) -> list[Piece]: ...

    @property
    @abstractmethod
    def active_player(self) -> Color: ...

    @property
    @abstractmethod
    def in_check(self) -> bool: ...

    @property
    @abstractmethod
    def game_result(self) -> GameResult: ...


class IPlayer(Protocol):
    player_type: PlayerType | str

    @abstractmethod
    def decide_on_move(self, board: Board) -> Move: ...
