from typing import Optional

from PyQt5.QtCore import QPoint, Qt, pyqtSignal
from PyQt5.QtGui import QColor, QMouseEvent, QPainter, QPen
from PyQt5.QtWidgets import QGraphicsDropShadowEffect, QSizePolicy, QWidget
from wrappers import Move, Piecetype, Square

from schaak_plezier.config import SETTINGS
from schaak_plezier.gui import Mode
from schaak_plezier.interface.observe import ObserverWidget
from schaak_plezier.objects.chessboard import Chessboard
from schaak_plezier.widgets.piece import Piece


class ChessboardView(ObserverWidget):
    validMoveClicked = pyqtSignal(Move)
    squareClickedInEditMode = pyqtSignal(Square)
    addPiece = pyqtSignal(Piece)

    selected_square: Optional[Square]
    previous_move: Optional[Move]
    selected_piece_moves: list[Move]
    wpieces: list[Piece]
    bpieces: list[Piece]
    legal_moves: list[Move]

    mode: Mode

    def __init__(self, board: Chessboard, parent: Optional[QWidget]):
        ObserverWidget.__init__(self, observable=board, parent=parent)
        self.mode = Mode.IDLE

        self.setMinimumSize(500, 500)
        self.setMaximumSize(1000, 1000)

        shadow = QGraphicsDropShadowEffect(self)
        shadow.setBlurRadius(10)
        shadow.setColor(QColor(0, 0, 0, 150))
        shadow.setOffset(3, 3)
        self.setGraphicsEffect(shadow)

        self.board_size = min(self.width(), self.height())
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

        self.selected_square: Optional[Square] = None
        self.previous_move: Optional[Move] = None
        self.selected_piece_moves: list[Move] = []
        self.wpieces: list[Piece] = []
        self.bpieces: list[Piece] = []
        self.legal_moves: list[Move] = []
        self.repaint()

    ##### DRAWING EVENTS #####
    def notify(self, board: Optional[Chessboard] = None, **kwargs):
        """Called by board when it changes"""
        if board is not None:
            self.wpieces = board.wpieces
            self.bpieces = board.bpieces
            self.legal_moves = board.possible_moves
            self.previous_move = board.history[-1] if len(board.history) > 0 else None
            self.update()

    def update(self, *args, **kwargs):
        """Called when an event happens/change occurs that changes the visuals, but not the underlying board object"""
        self.selected_piece_moves = (
            [move for move in self.legal_moves if move.fromSquare == self.selected_square]
            if self.selected_square is not None
            else []
        )
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
                    color = QColor(SETTINGS.colors.light_color.value)
                else:
                    color = QColor(SETTINGS.colors.dark_color.value)
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
            border_col=QColor(*SETTINGS.colors.selected_square.border.values),
            fill_col=QColor(*SETTINGS.colors.selected_square.fill.values),
        )

    def draw_selected_piece_moves(self, qp: QPainter):
        if not self.selected_piece_moves:
            return

        for move in self.selected_piece_moves:
            self.drawSquare(
                qp,
                square=move.targetSquare,
                border_col=QColor(*SETTINGS.colors.possible_moves.border.values),
                fill_col=QColor(*SETTINGS.colors.possible_moves.fill.values),
            )

    def draw_previous_move(self, qp: QPainter):
        if self.previous_move is None:
            return

        self.drawSquare(
            qp,
            self.previous_move.fromSquare,
            border_col=QColor(*SETTINGS.colors.previous_move.border.values),
            fill_col=QColor(*SETTINGS.colors.previous_move.fill.values),
        )
        self.drawSquare(
            qp,
            self.previous_move.targetSquare,
            border_col=QColor(*SETTINGS.colors.previous_move.border.values),
            fill_col=QColor(*SETTINGS.colors.previous_move.fill.values),
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
        match self.mode:
            case Mode.EDIT:
                self.handle_edit_mode_events(event)
            case Mode.PLAYING:
                self.handle_playing_mode_events(event)
            case Mode.IDLE:
                self.handle_idle_mode_events(event)
            case _:
                raise ValueError(f"Invalid mode: {self.mode}")

    def handle_idle_mode_events(self, event: QMouseEvent):
        pass

    def handle_edit_mode_events(self, event: QMouseEvent):
        clicked_square = self.getSquare(event.pos())
        if clicked_square:
            self.squareClickedInEditMode.emit(clicked_square)
            self.update()
        event.accept()

    def handle_playing_mode_events(self, event):
        clicked_square = self.getSquare(event.pos())
        if self.selected_square is None:
            self.selected_square = clicked_square
            self.update()
            event.accept()
        else:
            moveSquares = [move.targetSquare for move in self.selected_piece_moves]
            if clicked_square in moveSquares:
                for move in self.selected_piece_moves:
                    if move.targetSquare == clicked_square:
                        if move.isPromotion:
                            # TODO implement
                            move.promotionPiece = Piecetype.Queen

                        self.selected_square = None
                        event.accept()
                        self.update()
                        self.validMoveClicked.emit(move)
                        break
            else:
                self.selected_square = clicked_square
                self.update()
                event.accept()

    def mouseReleaseEvent(self, event: QMouseEvent, *args, **kwargs):
        if not self.mode == Mode.PLAYING:
            return
        if event.button() == Qt.RightButton:
            self.selected_square = None
            self.selected_piece_moves = []
            self.update()
            event.accept()
        else:
            event.ignore()

    def getSquare(self, pos: QPoint):
        square_size = self.board_size // 8
        col = pos.x() // square_size
        row = pos.y() // square_size

        if 0 <= row < 8 and 0 <= col < 8:
            return Square(8 * row + col)
        return None
