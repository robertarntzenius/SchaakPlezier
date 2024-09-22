from abc import abstractmethod
from typing import Protocol

from wrappers import Board, Move, PlayerType


class IPlayer(Protocol):
    @abstractmethod
    def getPlayerType(self) -> PlayerType: ...

    @abstractmethod
    def decideOnMove(self, board: Board) -> Move: ...
