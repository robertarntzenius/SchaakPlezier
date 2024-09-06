from typing import Tuple

import wrappers

from schaak_plezier.interface.config import SETTINGS
from schaak_plezier.interface.log import SchaakPlezierLogging
from schaak_plezier.interface.observe import ObservableWidget
from schaak_plezier.interface.wrapper_types import Color, GameResult, Move, Piecetype, Square
from schaak_plezier.model.piece import Piece


class Chessboard(ObservableWidget):
    _board: wrappers.Board

    def __init__(self):
        super().__init__()
        self.logger = SchaakPlezierLogging.getLogger(__name__)
        self._board = wrappers.Board(SETTINGS.fen_string, SETTINGS.log_file.as_posix())
        self.notify_observers(board=self)
        self.logger.info("Created chessboard")

    def do_move(self, move: Move) -> None:
        self._board.doMove(move.fromSquare, move.targetSquare, move.promotionPiece)
        self.notify_observers(board=self)

    def undo_move(self) -> None:
        if len(self.history) > 0:
            self._board.undoMove()
            self.notify_observers(board=self)

    def clear_board(self) -> None:
        self._board.clearBoard()
        self.notify_observers(board=self)

    def initialize_from_fen(self, fen_string) -> None:
        try:
            _ = wrappers.Board(fen_string, "tmp.log")
        except Exception:
            raise ValueError(f"Invalid FEN string: {fen_string}")
        self._board.clearBoard()
        self._board.initFromFEN(fen_string)
        self.notify_observers(board=self)

    def validate(self) -> Tuple[bool, list[str]]:
        check = self._board.validate()

        valid: bool = check[0]
        error_description: str = check[1]

        if valid:
            self.notify_observers(board=self)
        return (valid, error_description)

    def add_piece(self, color: Color, piecetype: Piecetype, square: Square) -> None:
        self.logger.debug(f"{color}, {piecetype}, {square}")
        if self.find_piece(square):
            self.remove_piece(square)
        self._board.addPiece(color.value, piecetype.value, square.value)
        self.notify_observers(board=self)

    def find_piece(self, square) -> Piece | None:
        if square in self.wpieces:
            return self.wpieces[square]
        elif square in self.bpieces:
            return self.bpieces[square]

    def remove_piece(self, square: Square) -> None:
        if piece := self.find_piece(square):
            self._board.removePiece(piece.piece_type.value, piece.color.value, piece.square.value)
            self.notify_observers(board=self)

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
        self.logger.debug(self._board.getGameResult(no_legal_moves))
        return GameResult(self._board.getGameResult(no_legal_moves))
