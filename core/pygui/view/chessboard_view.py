from PyQt5.QtWidgets import QSizePolicy, QGraphicsDropShadowEffect
from PyQt5.QtGui import QPainter, QColor, QPen
from PyQt5.QtCore import Qt, pyqtSignal

from core.pygui.controller.controller import Controller, Mode
from core.pygui.model.piece import Piece
from core.pygui.model.player import Player
from core.pygui.model.wrapper_types import Color, Move, Piecetype, PlayerType, Square

from .observer import ObserverWidget


class ChessboardView(ObserverWidget):
    validMoveClicked = pyqtSignal(Move)

    def __init__(self, controller: Controller, config: dict, parent=None):
        super().__init__(observable_list=[controller.board], parent=parent)
        self.setMinimumSize(500, 500)
        self.setMaximumSize(1000, 1000)
        self.config = config
        self.controller = controller

        shadow = QGraphicsDropShadowEffect(self)
        shadow.setBlurRadius(10)
        shadow.setColor(QColor(0, 0, 0, 150))
        shadow.setOffset(3, 3)
        self.setGraphicsEffect(shadow)

        self.board_size = min(self.width(), self.height())
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        
        self.selected_square = None
        self.previous_move: Move = None
        self.selected_piece_moves = []
        self.wpieces = []
        self.bpieces = []
        self.legal_moves = []

    ##### DRAWING EVENTS #####
    def notify(self, board = None):
        """Called by board when it changes"""
        if board is not None:
            self.wpieces = board.wpieces
            self.bpieces = board.bpieces
            self.legal_moves = board.possible_moves
            self.previous_move = board.history[-1] if len(board.history) > 0 else None
            self.update()

    def update(self):
        """Called by Widget when an event happens/change occurs that changes the visuals, but not the underlying board object"""
        self.selected_piece_moves = [move for move in self.legal_moves if move.fromSquare == self.selected_square] if self.selected_square is not None else []
        self.repaint()

    def paintEvent(self, event):
        """What is executed when a draw call is made"""
        qp = QPainter(self)
        self.drawChessBoard(qp)
        self.drawEventIndicators(qp)
        self.drawPieces(qp)
        qp.end()

    def drawEventIndicators(self, qp):
        self.draw_previous_move(qp)
        self.draw_selected_piece_moves(qp)
        self.draw_selected_square(qp)

    def drawChessBoard(self, qp):
        self.board_size = min(self.width(), self.height())
        cell_size = self.board_size // 8

        for row in range(8):
            for col in range(8):
                if (row + col) % 2 == 0:
                    color = QColor(self.config.gui.light_color)
                else:
                    color = QColor(self.config.gui.dark_color)
                qp.fillRect(row * cell_size, col * cell_size, cell_size, cell_size, color)
        
    def drawPieces(self, qp):        
        for piece_entry in self.wpieces:
            square, piece_type = piece_entry
            piece = Piece(square, piece_type, Color('White'))
            piece.draw(qp, self.board_size)
        
        for piece_entry in self.bpieces:
            square, piece_type = piece_entry
            piece = Piece(square, piece_type, Color('Black'))
            piece.draw(qp, self.board_size)

    def draw_selected_square(self, qp):
        if self.selected_square is None:
            return
        self.drawSquare(qp, self.selected_square, border_col=QColor(*self.config.gui.selected_square.border), fill_col=QColor(*self.config.gui.selected_square.fill))

    def draw_selected_piece_moves(self, qp):
        if not self.selected_piece_moves:
            return
        
        for move in self.selected_piece_moves:
            self.drawSquare(qp, square=move.targetSquare, border_col=QColor(*self.config.gui.possible_moves.border), fill_col=QColor(*self.config.gui.possible_moves.fill))

    def draw_previous_move(self, qp):
        if self.previous_move is None:
            return

        self.drawSquare(qp, self.previous_move.fromSquare, border_col=QColor(*self.config.gui.previous_move.border), fill_col=QColor(*self.config.gui.previous_move.fill))
        self.drawSquare(qp, self.previous_move.targetSquare, border_col=QColor(*self.config.gui.previous_move.border), fill_col=QColor(*self.config.gui.previous_move.fill))

    def drawSquare(self, qp, square, border_col: QColor=None, border_width: int=3, fill_col: QColor=None):
        square_size = self.board_size // 8
        
        border_col = border_col or Qt.blue

        pen = QPen(border_col)
        pen.setWidth(border_width)
        qp.setPen(pen)

        if fill_col is not None:
            qp.setBrush(fill_col)

        col = square.value % 8
        row = square.value // 8
        qp.drawRect(col * square_size, row * square_size, square_size, square_size)

        
    ##### MOUSE EVENTS #####
    def mousePressEvent(self, event):
        if self.controller.mode == Mode.EDIT:
            self.handle_edit_mode_events(event)
        elif self.controller.mode == Mode.PLAYING:
            self.handle_playing_mode_events(event)
        else:
            self.handle_idle_events(event)
    
    def handle_idle_events(event):
        pass

    def handle_edit_mode_events(self, event):
        clicked_square = self.getSquare(event.pos())
        self.selected_square = clicked_square
        # get/update the selected piece 
        self.place_piece(clicked_square)
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
                            move.promotionPiece = Piecetype('Queen').to_cpp_object()

                        self.selected_square = None
                        event.accept()
                        self.update()
                        self.validMoveClicked.emit(move)
                        break
            else:
                self.selected_square = clicked_square
                self.update()
                event.accept()

    def mouseReleaseEvent(self, event):
        if not self.controller.mode == Mode.PLAYING:
            return
        if event.button() == Qt.RightButton:
            self.selected_square = None
            self.selected_piece_moves = []
            self.update()
            event.accept()
        else:
            event.ignore()

    def getSquare(self, pos):
        square_size = self.board_size // 8
        col = pos.x() // square_size
        row = pos.y() // square_size

        if 0 <= row < 8 and 0 <= col < 8:
            return Square(8 * row + col)
        return None