from typing import Optional

from PyQt5.QtCore import Qt, pyqtSignal
from PyQt5.QtGui import QMouseEvent, QPixmap
from PyQt5.QtWidgets import QDialog, QGridLayout, QLabel, QPushButton, QVBoxLayout, QWidget
from wrappers import Color, Piecetype, Square

from schaak_plezier.widgets.piece import Piece


class ClickablePieceLabel(QLabel):
    clicked = pyqtSignal(Piece)
    piece: Piece

    def __init__(self, color: Color, type: Piecetype, parent=None):
        super().__init__(parent=parent)
        self.piece = Piece(Square.NoSquare, type, color)

        pixmap = QPixmap.fromImage(self.piece.image).scaled(50, 50)
        self.setPixmap(pixmap)
        self.setAlignment(Qt.AlignmentFlag.AlignCenter)

    def mousePressEvent(self, event: QMouseEvent, *args, **kwargs):
        self.clicked.emit(self.piece)


class EditBoardDialog(QDialog):
    pieceToAdd_signal = pyqtSignal(Piece)  # Signal emitted when a piece is placed
    boardCleared_signal = pyqtSignal()
    tryValidate_signal = pyqtSignal()

    def __init__(self, parent: Optional[QWidget]):
        super().__init__(parent=parent)
        self.gui = parent

        self.setWindowTitle("Edit Board")

        layout = QVBoxLayout()

        clear_button = QPushButton("Clear Board")
        clear_button.clicked.connect(lambda: self.boardCleared_signal.emit())
        layout.addWidget(clear_button)

        # Piece selection grid
        piece_selection_layout = QGridLayout()
        self.piece_buttons: dict[Piece, ClickablePieceLabel] = {}

        colors = [Color.White, Color.Black]
        piece_types = [
            Piecetype.Pawn,
            Piecetype.Knight,
            Piecetype.Bishop,
            Piecetype.Rook,
            Piecetype.Queen,
            Piecetype.King,
        ]
        self.piece_to_add = None

        for col, color in enumerate(colors):
            for row, type in enumerate(piece_types):
                label = ClickablePieceLabel(color, type, parent=self)
                label.clicked.connect(self.select_piece)
                piece_selection_layout.addWidget(label, row, col)
                self.piece_buttons[label.piece] = label

        layout.addLayout(piece_selection_layout)

        # Validate button
        self.validate_button = QPushButton("Validate")
        self.validate_button.clicked.connect(lambda: self.tryValidate_signal.emit())
        layout.addWidget(self.validate_button)

        self.setLayout(layout)

    def select_piece(self, piece: Piece):
        if self.piece_to_add:
            old_piece_label = self.piece_buttons[self.piece_to_add]
            old_piece_label.setStyleSheet("")
        self.piece_to_add = piece
        piece_label = self.piece_buttons[self.piece_to_add]
        piece_label.setStyleSheet("border: 2px solid red;")
        self.pieceToAdd_signal.emit(self.piece_to_add)
