from PyQt5.QtWidgets import QWidget, QSizePolicy, QMessageBox, QDialog, QGraphicsDropShadowEffect
from PyQt5.QtGui import QPainter, QColor
from PyQt5.QtCore import Qt

from model.wrapper_types import Square, Piecetype, Color
from model.chessboard import Chessboard
from model.piece import Piece

from .observer import ObserverWidget
from .edit_players_dialog import PlayerSelectDialog
from .edit_fen_dialog import FenInputDialog
from .errordialog import ErrorDialog


class ChessboardView(ObserverWidget):
    def __init__(self, board: Chessboard, parent=None):
        super().__init__(board, parent=parent)
        self.setMinimumSize(500, 500)
        self.setMaximumSize(1000, 1000)
        self.board = board

        shadow = QGraphicsDropShadowEffect(self)
        shadow.setBlurRadius(10)
        shadow.setColor(QColor(0, 0, 0, 150))
        shadow.setOffset(3, 3)
        self.setGraphicsEffect(shadow)

        self.board_size = min(self.width(), self.height())
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        self.playing = False
        
        self.selected_square = None
        self.selected_piece_moves = []
        self.wpieces = self.board.wpieces
        self.bpieces = self.board.bpieces
        self.legal_moves = self.board.possible_moves

    
    ##### DRAWING EVENTS #####
    def notify(self):
        """Called by board when it changes"""
        self.wpieces = self.board.wpieces
        self.bpieces = self.board.bpieces
        self.legal_moves = self.board.possible_moves
        self.update()

    def update(self):
        """Called by Widget when an event happens/change occurs that changes the visuals, but not the underlying board object"""
        self.selected_piece_moves = [move for move in self.legal_moves if move.fromSquare == self.selected_square] if self.selected_square is not None else []
        self.repaint()

    def paintEvent(self, event):
        """What is executed when a draw call is made"""
        qp = QPainter(self)
        self.drawChessBoard(qp)
        self.drawPieces(qp)
        self.drawEventIndicators(qp)
        qp.end()

    def drawEventIndicators(self, qp):
        self.drawSelectedSquare(qp)
        self.draw_selected_piece_moves(qp)

    def drawChessBoard(self, qp):
        self.board_size = min(self.width(), self.height())
        cell_size = self.board_size // 8

        for row in range(8):
            for col in range(8):
                if (row + col) % 2 == 0:
                    color = QColor("#f0d9b5")
                else:
                    color = QColor("#946f51")
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

    def drawSelectedSquare(self, qp):
        if self.selected_square is None:
            return
        square_size = self.board_size // 8
        qp.setPen(Qt.red)
        col = self.selected_square.value % 8
        row = self.selected_square.value // 8
        qp.drawRect(col * square_size, row * square_size, square_size, square_size)

    def draw_selected_piece_moves(self, qp):
        if not self.selected_piece_moves:
            return

        qp.setPen(Qt.blue)
        qp.setBrush(QColor(255, 200, 200, 100))
        for move in self.selected_piece_moves:
            square_size = self.board_size // 8
            targetSquare = move.targetSquare
            col = targetSquare.value % 8
            row = targetSquare.value // 8
            qp.drawRect(col * square_size, row * square_size, square_size, square_size)
    
    ##### MOUSE EVENTS #####
    def mousePressEvent(self, event):
        if not self.playing:
            return
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
                        self.board.do_move(move)
                        self.selected_square = None
                        self.update()
                        event.accept()
                        break
            else:
                self.selected_square = clicked_square
                self.update()
                event.accept()

    def mouseReleaseEvent(self, event):
        if not self.playing:
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

    ###### Game/Button Commands ######
    def start_game(self):
        self.playing = True
        QMessageBox.information(self, "Game Started", f"{self.board.active_player} to move.")

    def resign(self):
        self.selected_square = None
        self.selected_piece_moves = []
        self.playing = False
        QMessageBox.information(self, "Game Over", f"{self.board.active_player} resigned.")
        self.update()

    def edit_board(self):
        pass
        # TODO allow user to set up board
    
    def edit_fen_dialog(self):
        if self.playing:
            error_dialog = ErrorDialog("Cannot edit the fen while game is ongoing.", self)
            error_dialog.exec_()
            return
        while True:
            dialog = FenInputDialog(self)
            if dialog.exec_() == QDialog.Accepted:
                fen_string = dialog.get_fen_string()
                try:
                    self.board.initialize_from_fen(fen_string)
                    break
                except ValueError as e:
                    error_dialog = ErrorDialog(str(e), self)
                    error_dialog.exec_()
            else: # Cancel
                break
        
    def edit_players(self):
        if self.playing:
            error_dialog = ErrorDialog("Cannot edit the players while game is ongoing.", self)
            error_dialog.exec_()
            return
        while True:
            dialog = PlayerSelectDialog(self)
            if dialog.exec_() == QDialog.Accepted:
                white_player, black_player = dialog.get_player_types()
                print(white_player, black_player)
                break
            else: # Cancel
                break