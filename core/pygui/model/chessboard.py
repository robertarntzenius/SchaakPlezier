import logging
import wrappers
from .wrapper_types import Move, Piecetype, Square, GameResult, Color
from .observable import Observable
from .piece import Piece

class Chessboard(Observable):
    _board: wrappers.Board

    def __init__(self, config: dict):
        super().__init__()
        self.config = config
        self._board = wrappers.Board(self.config.defaults.fen_string, self.config.defaults.log_file)
        self.notify_observers(board=self)
        logging.info('Created chessboard')

    def do_move(self, move: Move):
        self._board.doMove(move.fromSquare, move.targetSquare, move.promotionPiece)
        self.notify_observers(board=self)

    def undo_move(self):
        if len(self.history) > 0:
            self._board.undoMove()
            self.notify_observers(board=self)
    
    def clear_board(self):
        self._board.clearBoard()

    def initialize_from_fen(self, fen_string):
        try:
            cpy_board = wrappers.Board(fen_string, 'tmp.log')
        except Exception as e:
            raise ValueError(f"Invalid FEN string: {fen_string}")
        self._board.clearBoard()
        self._board.initFromFEN(fen_string)
        self.notify_observers(board=self)

    def add_piece(self, color: Color, square: Square, piecetype: Piecetype):
        self._board.addPiece(color, Square('NoSquare'), Square(square))

    def _getPossibleMoves(self):
        return self._board.getPossibleMoves()
    
    @property
    def possible_moves(self) -> list[Move]:
        return [Move(move) for move in self._board.getPossibleMoves()]

    @property
    def wpieces(self) -> list[Piece]:
        return self._board.getPieceMaps()[0]
    
    @property
    def bpieces(self) -> list[Piece]:
        return self._board.getPieceMaps()[1]

    @property   
    def history(self) -> list[Move]:
        return self._board.getHistory()
    
    @property
    def active_player(self) -> Color:
        return Color(self._board.getBoardState().activePlayer)
    
    @property
    def in_check(self) -> bool:
        return self._board.inCheck()
    
    @property
    def game_result(self) -> GameResult:
        no_legal_moves = len(self.possible_moves) == 0
        return GameResult(self._board.getGameResult(no_legal_moves))
    
