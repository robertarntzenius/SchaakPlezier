from abc import abstractmethod
from typing import Protocol

from wrappers import Move, PlayerType


class IPlayer(Protocol):
    player_type: PlayerType

    @abstractmethod
    def decideOnMove(self, *args, **kwargs) -> Move: ...
