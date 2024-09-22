from pathlib import Path
from typing import Optional, Tuple

from PyQt5.QtCore import QObject, pyqtSignal
from wrappers import Board, Color, GameResult, Move, Square

from schaak_plezier.log import SchaakPlezierLogging
from schaak_plezier.widgets.piece import Piece


class Chessboard(QObject):
    boardChanged = pyqtSignal(QObject)  # self emitter

    redo_list: list[Move] = []

    def __init__(self, fen_string: str, log_file: Optional[Path] = None):
        super().__init__()
        self.logger = SchaakPlezierLogging.getLogger(__name__)

        _file = str(log_file) if log_file else ""
        self._board = Board(fen_string, _file)
        self.boardChanged.emit(self)
        self.logger.info("Created chessboard")

    @property
    def possible_moves(self) -> list[Move]:
        return self._board.getPossibleMoves()

    @property
    def wpieces(self) -> list[Piece]:
        return self._getPieceList(Color.White)

    @property
    def bpieces(self) -> list[Piece]:
        return self._getPieceList(Color.Black)

    @property
    def history(self) -> list[Move]:
        return self._board.getHistory()

    @property
    def active_player(self) -> Color:
        return self._board.getBoardState().activePlayer

    @property
    def in_check(self) -> bool:
        return self._board.inCheck()

    @property
    def game_result(self) -> GameResult:
        no_legal_moves = len(self.possible_moves) == 0
        return self._board.getGameResult(no_legal_moves)

    def do_move(self, move: Move) -> None:
        self._board.doMove(move.fromSquare, move.targetSquare, move.promotionPiece)
        self.boardChanged.emit(self)

    def undo_move(self) -> None:
        if len(self.history) > 0:
            self.redo_list.append(self.history[-1])
            self._board.undoMove()
            self.boardChanged.emit(self)

    def redo_move(self) -> None:
        if len(self.redo_list) > 0:
            self.do_move(self.redo_list.pop())

    def clear_board(self) -> None:
        self._board.clearBoard()
        self.boardChanged.emit(self)

    def initialize_from_fen(self, fen_string: str) -> None:
        try:
            _ = Board(fen_string, "")
        except Exception:
            raise ValueError(f"Invalid FEN string: {fen_string}")
        self._board.clearBoard()
        self._board.initFromFEN(fen_string)
        self.boardChanged.emit(self)

    def validate(self) -> Tuple[bool, list[str]]:
        valid, error_list = self._board.validate()

        if valid:
            self.boardChanged.emit(self)
        return (valid, error_list)

    def add_piece(self, piece: Piece) -> None:
        self.logger.debug(f"{piece.color}, {piece.piece_type}, {piece.square}")
        self._board.addPiece(color=piece.color, piecetype=piece.piece_type, square=piece.square)
        self.boardChanged.emit(self)

    def find_piece(self, square: Square) -> Piece | None:
        if self._board.findPiece(square):
            color, ptype, square = self._board.getPiece(square)
            return Piece(color=color, piece_type=ptype, square=square)

    def remove_piece(self, square: Square) -> None:
        if self.find_piece(square):
            color, ptype, square = self._board.getPiece(square)
            self._board.removePiece(color=color, piecetype=ptype, square=square)
            self.boardChanged.emit(self)

    def _getPieceList(self, color: Color) -> list[Piece]:
        return [
            Piece(Square(square), piecetype, color)  # square is an int here
            for square, piecetype in self._board.getPieceMap(color).items()
        ]
