import wrappers
from .piece import Piece

from .wrapper_types import Square, Move, Piecetype, Color

from .observable import Observable
from .definitions import *


class Chessboard(Observable):
    _board: wrappers.Board

    def __init__(self, fen_string = DEFAULT_FEN, log_file = DEFAULT_LOG_FILE):
        super().__init__()
        self._board = wrappers.Board(fen_string, log_file)

    def do_move(self, move: Move):
        self._board.doMove(move.fromSquare, move.targetSquare, move.promotionPiece)
        self.notify_observers()

    def undo_move(self):
        self._board.undoMove()
        self.notify_observers()
    
    def initialize_from_fen(self, fen_string):
        try:
            cpy_board = wrappers.Board(fen_string, 'tmp.log')
        except Exception as e:
            raise ValueError(f"Invalid FEN string: {fen_string}")

        self._board.clearBoard()
        self._board.initializeFromFEN(fen_string)
        self.notify_observers()

    @property
    def possible_moves(self):
        return [Move(move) for move in self._board.getPossibleMoves()]

    @property
    def wpieces(self):
        return self._board.getPieceMaps()[0]
    
    @property
    def bpieces(self):
        return self._board.getPieceMaps()[1]

    @property   
    def history(self):
        return self._board.getHistory()
    
    @property
    def active_player(self):
        return self._board.getBoardState().activePlayer.name
    
