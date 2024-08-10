from abc import abstractmethod
from typing import Protocol

import wrappers
from schaak_plezier.interface.observe import ObservableWidget
from schaak_plezier.interface.wrapper_types import Move, PlayerType


class IChessboard(ObservableWidget):
    _board: wrappers.Board

    @abstractmethod
    def do_move(self, move: Move) -> None: ...

    @abstractmethod
    def undo_move(self) -> None: ...


class IPlayer(Protocol):
    player_type: PlayerType

    @abstractmethod
    def decide_on_move(self, board: IChessboard) -> Move: ...
