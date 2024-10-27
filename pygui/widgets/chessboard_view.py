from typing import Optional

from PyQt5.QtCore import Qt, pyqtSignal
from PyQt5.QtGui import QColor, QMouseEvent, QPainter, QPen
from PyQt5.QtWidgets import QGraphicsDropShadowEffect, QSizePolicy, QWidget
from wrappers import Move, Piecetype, Square

from pygui.config import ColorConfig, Mode
from pygui.objects.chessboard import Chessboard
from pygui.widgets.piece import Piece


class ChessboardView(QWidget):
    validMoveClicked = pyqtSignal(Move)
    squareClicked = pyqtSignal(Square)
    squareClickedInEditMode = pyqtSignal(Square)

    previous_move: Optional[Move]
    wpieces: list[Piece]
    bpieces: list[Piece]
    legal_moves: list[Move]

    colors: ColorConfig
    mode: Mode = Mode.IDLE

    _selected_piece_moves: list[Move] = []
    _selected_square: Optional[Square] = None

    def __init__(self, board: Chessboard, color_config: ColorConfig, parent: Optional[QWidget]):
        QWidget.__init__(self, parent=parent)
        board.boardChanged.connect(lambda board: self.handle_board_changed(board))

        self.colors = color_config

        self.setMinimumSize(500, 500)
        self.setMaximumSize(1000, 1000)

        shadow = QGraphicsDropShadowEffect(self)
        shadow.setBlurRadius(10)
        shadow.setColor(QColor(0, 0, 0, 150))
        shadow.setOffset(3, 3)
        self.setGraphicsEffect(shadow)

        self.board_size = min(self.width(), self.height())
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

        self.previous_move = board.history[-1] if len(board.history) > 0 else None
        self.wpieces = board.wpieces
        self.bpieces = board.bpieces
        self.legal_moves = board.possible_moves

        self.squareClicked.connect(lambda square: self.handle_square_clicked(square))

        self.repaint()

    def set_mode(self, mode: Mode):
        self.mode = mode

    @property
    def selected_piece_moves(self) -> list[Move]:
        return self._selected_piece_moves

    @selected_piece_moves.setter
    def selected_piece_moves(self, moves: list[Move]):
        self._selected_piece_moves = moves

    @property
    def selected_square(self) -> Optional[Square]:
        return self._selected_square

    @selected_square.setter
    def selected_square(self, square: Optional[Square]):
        if square == self._selected_square:
            return
        self._selected_square = square
        self.selected_piece_moves = [move for move in self.legal_moves if move.fromSquare == square]
        self.repaint()

    ##### DRAWING EVENTS #####
    def handle_board_changed(self, board: Chessboard):
        self.wpieces = board.wpieces
        self.bpieces = board.bpieces
        self.legal_moves = board.possible_moves
        self.previous_move = board.history[-1] if len(board.history) > 0 else None

        self.selected_square = None

        self.update()
        self.repaint()

    def paintEvent(self, event, *args, **kwargs):
        """What is executed when a draw call is made"""
        qp = QPainter(self)
        self.drawChessBoard(qp)
        self.drawEventIndicators(qp)
        self.drawPieces(qp)
        qp.end()

    def drawEventIndicators(self, qp: QPainter):
        self.draw_previous_move(qp)
        self.draw_selected_piece_moves(qp)
        self.draw_selected_square(qp)

    def drawChessBoard(self, qp: QPainter):
        self.board_size = min(self.width(), self.height())
        cell_size = self.board_size // 8

        for row in range(8):
            for col in range(8):
                if (row + col) % 2 == 0:
                    color = QColor(self.colors.light_color.value)
                else:
                    color = QColor(self.colors.dark_color.value)
                qp.fillRect(row * cell_size, col * cell_size, cell_size, cell_size, color)

    def drawPieces(self, qp: QPainter):
        for piece in self.wpieces:
            piece.draw(qp, self.board_size)

        for piece in self.bpieces:
            piece.draw(qp, self.board_size)

    def draw_selected_square(self, qp: QPainter):
        if self.selected_square is None:
            return
        self.drawSquare(
            qp,
            self.selected_square,
            border_col=QColor(*self.colors.selected_square.border.values),
            fill_col=QColor(*self.colors.selected_square.fill.values),
        )

    def draw_selected_piece_moves(self, qp: QPainter):
        if not self.selected_piece_moves:
            return

        for move in self.selected_piece_moves:
            self.drawSquare(
                qp,
                square=move.targetSquare,
                border_col=QColor(*self.colors.possible_moves.border.values),
                fill_col=QColor(*self.colors.possible_moves.fill.values),
            )

    def draw_previous_move(self, qp: QPainter):
        if self.previous_move is None:
            return

        self.drawSquare(
            qp,
            self.previous_move.fromSquare,
            border_col=QColor(*self.colors.previous_move.border.values),
            fill_col=QColor(*self.colors.previous_move.fill.values),
        )
        self.drawSquare(
            qp,
            self.previous_move.targetSquare,
            border_col=QColor(*self.colors.previous_move.border.values),
            fill_col=QColor(*self.colors.previous_move.fill.values),
        )

    def drawSquare(
        self,
        qp: QPainter,
        square: Square,
        border_col: QColor = Qt.blue,
        border_width: int = 3,
        fill_col: Optional[QColor] = None,
    ):
        square_size = self.board_size // 8

        pen = QPen(border_col)
        pen.setWidth(border_width)
        qp.setPen(pen)

        if fill_col is not None:
            qp.setBrush(fill_col)

        col = square.value % 8
        row = square.value // 8
        qp.drawRect(col * square_size, row * square_size, square_size, square_size)

    ##### MOUSE EVENTS #####
    def mousePressEvent(self, event: QMouseEvent, *args, **kwargs):
        if not (clicked_square := self.get_square_from_mouse_event(event)):
            return
        self.squareClicked.emit(clicked_square)

        self.update()
        self.repaint()
        event.accept()

    #### EVENT HANDLERS #####
    def handle_square_clicked(self, square: Square):
        match self.mode:
            case Mode.PLAYING:
                if move := self.can_play_move(square):
                    self.validMoveClicked.emit(move)
                    self.selected_square = None
                else:
                    self.selected_square = square
            case Mode.EDIT:
                self.squareClickedInEditMode.emit(square)
            case Mode.IDLE:
                pass
            case _:
                raise ValueError(f"Invalid mode: {self.mode}")

    def can_play_move(self, clicked_square: Square) -> Move | None:
        if clicked_square not in [move.targetSquare for move in self.selected_piece_moves]:
            return

        # TODO implement promotion
        move = [move for move in self.selected_piece_moves if move.targetSquare == clicked_square][
            0
        ]
        if move.isPromotion:
            move.promotionPiece = Piecetype.Queen

        return move

    def get_square_from_mouse_event(self, event: QMouseEvent) -> Square | None:
        square_size = self.board_size // 8

        col = event.pos().x() // square_size
        row = event.pos().y() // square_size

        if 0 <= row < 8 and 0 <= col < 8:
            return Square(8 * row + col)
        return None
