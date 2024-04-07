

from abc import abstractmethod
from typing import Protocol
from .observable import Observable
from .wrapper_types import Move, Piecetype, PlayerType
from .chessboard import Chessboard
from PyQt5.QtCore import QEventLoop

import wrappers

class IPlayer(Protocol):
    player_type: PlayerType

    @abstractmethod
    def decide_on_move(self, board: Chessboard) -> Move:
        ...
    
class Player(IPlayer):
    def __init__(self, player_type: str):
        if player_type.lower() == 'human':
            raise ValueError('Should not make Human players with this Player class. Use HumanPlayer() instead')
        self.player_type = PlayerType(player_type)
        self._player = wrappers.makePlayer(player_type)
    
    def decide_on_move(self, board: Chessboard) -> Move:
        moves = board.possible_moves
        ind = self._player.decideOnMove(board._board, board._getPossibleMoves())
        if ind > len(moves):
            raise IndexError('Got invalid index from cpp obj')
        return moves[ind]


class HumanPlayer(IPlayer):
    def __init__(self, chessboard_view):
        super().__init__()
        self.player_type = PlayerType('Human')
        self.chessboard_view = chessboard_view
        self.chessboard_view.validMoveClicked.connect(self.handle_valid_move_clicked)
        self.clicked_move = None
        self.event_loop = QEventLoop()

    def decide_on_move(self, board: Chessboard) -> Move:
        # This func gets called by controller
        # Wait for the signal emitted by chessboard view: self.validMoveClicked.emit(move) (e.g. human decided on a move)
        # use the value of the signal to make the move.
        self.event_loop.exec_()
        return self.clicked_move
    
    def handle_valid_move_clicked(self, move: Move) -> None:
        self.clicked_move = move
        self.event_loop.quit()

