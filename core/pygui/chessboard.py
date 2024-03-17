from PyQt5.QtWidgets import QWidget, QSizePolicy
from PyQt5.QtGui import QPainter, QColor
from PyQt5.QtCore import Qt

from piece import Piece
from wrapper_types import Square, Move
import wrappers

DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
DEFAULT_LOG_FILE = "GUI.log"

class ChessBoard(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.board = wrappers.Board(DEFAULT_FEN, DEFAULT_LOG_FILE)
        self.setMinimumSize(500, 500)
        self.board_size = min(self.width(), self.height())
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

        self.wpieces = self.board.getPieceMaps()[0]
        self.bpieces = self.board.getPieceMaps()[1]
        self.legal_moves = []
        self.selected_square = None
        self.selected_piece = None
        self.selected_piece_moves = []


    def update(self):
        self.wpieces = self.board.getPieceMaps()[0]
        self.bpieces = self.board.getPieceMaps()[1]
        self.legal_moves = [Move(move) for move in self.board.getPossibleMoves()]
        self.selected_piece_moves = [move for move in self.legal_moves if move.fromSquare == self.selected_square] if self.selected_square is not None else []
        print("Update")
        print("legalmoves: ", self.legal_moves)
        print("selected_square: ", self.selected_square)
        print("selected_piecemoves: ", self.selected_piece_moves)
        self.repaint()


    def drawChessBoard(self, qp):
        self.board_size = min(self.width(), self.height())
        cell_size = self.board_size // 8

        # Draw board squares
        for row in range(8):
            for col in range(8):
                if (row + col) % 2 == 0:
                    color = Qt.white
                else:
                    color = Qt.darkGray
                qp.fillRect(row * cell_size, col * cell_size, cell_size, cell_size, color)

    def drawPieces(self, qp):        
        for piece_entry in self.wpieces:
            square, piece_type = piece_entry
            piece = Piece(square, piece_type, wrappers.Color.White)
            piece.draw(qp, self.board_size)
        
        for piece_entry in self.bpieces:
            square, piece_type = piece_entry
            piece = Piece(square, piece_type, wrappers.Color.Black)
            piece.draw(qp, self.board_size)

    def resizeEvent(self, event):
        self.board_size = min(self.width(), self.height())
        self.update()

    def mousePressEvent(self, event):
        clicked_square = self.getSquare(event.pos())
        clicked_piece = self.getPiece(clicked_square)

        # Not clicked on anything yet
        if self.selected_square is None:
            self.selected_square = clicked_square
            self.update()
            event.accept()
        else:
            # selected square exists.
            moveSquares = [move.targetSquare for move in self.selected_piece_moves]
            if clicked_square not in moveSquares:
                self.selected_square = clicked_square
                self.update()
                event.accept()
            else:
                for move in self.selected_piece_moves:
                    if move.targetSquare == clicked_square:
                        print("doMove: ", move)
                        self.board.doMove(move.fromSquare, move.targetSquare)
                        self.selected_square = None
                        self.update()
                        event.accept()
                        break


    def mouseReleaseEvent(self, event):
        if event.button() == Qt.RightButton:
            self.selected_square = None
            self.selected_piece_moves = []
            self.update()
            event.accept()
        else:
            event.ignore()  # Propagate the event to the parent widget

    def getSquare(self, pos):
        # Get the square corresponding to the mouse click position
        square_size = self.board_size // 8
        col = pos.x() // square_size
        row = pos.y() // square_size

        if 0 <= row < 8 and 0 <= col < 8:
            return Square(value=(8 * row + col))
        return None

    def getPiece(self, square: Square):
        for entry in self.wpieces:
            sq, _ = entry
            if Square(sq) == square:
                return entry, wrappers.Color.White
            
        for entry in self.bpieces:
            sq, _ = entry
            if sq == square:
                return entry, wrappers.Color.Black
        return None

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

    def drawSelectedSquare(self, qp):
        if not self.selected_square:
            return
        square_size = self.board_size // 8
        qp.setPen(Qt.red)
        col = self.selected_square.value % 8
        row = self.selected_square.value // 8
        qp.drawRect(col * square_size, row * square_size, square_size, square_size)

    def drawEventIndicators(self, qp):
        # Draw indicators for selected square and legal moves
        self.drawSelectedSquare(qp)
        self.draw_selected_piece_moves(qp)

    def paintEvent(self, event):
        qp = QPainter(self)
        self.drawChessBoard(qp)
        self.drawPieces(qp)
        self.drawEventIndicators(qp)
        qp.end()


