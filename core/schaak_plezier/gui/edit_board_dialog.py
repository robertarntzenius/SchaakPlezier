from PyQt5 import QtCore
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtGui import QPixmap
from PyQt5.QtWidgets import QDialog, QGridLayout, QLabel, QPushButton, QVBoxLayout

from schaak_plezier.interface.app import IGUI
from schaak_plezier.interface.wrapper_types import Color, Piecetype, Square
from schaak_plezier.model.piece import Piece


class ClickablePieceLabel(QLabel):
    clicked = pyqtSignal(Piece)
    piece: Piece

    def __init__(self, color: Color, type: Piecetype, parent=None):
        super().__init__(parent)
        self.piece = Piece(Square("NoSquare"), type, color)

        pixmap = QPixmap.fromImage(self.piece.image).scaled(50, 50)
        self.setPixmap(pixmap)
        self.setAlignment(QtCore.Qt.AlignCenter)

    def mousePressEvent(self, event):
        self.clicked.emit(self.piece)


class EditBoardDialog(QDialog):
    pieceToAdd_signal = pyqtSignal(Color, Piecetype)  # Signal emitted when a piece is placed
    boardCleared_signal = pyqtSignal()
    tryValidate_signal = pyqtSignal()

    def __init__(self, gui: IGUI, parent=None):
        super().__init__(parent)
        self.gui = gui

        self.setWindowTitle("Edit Board")

        layout = QVBoxLayout()

        self.clear_button = QPushButton("Clear Board")
        self.clear_button.clicked.connect(self.clear_board)
        layout.addWidget(self.clear_button)

        # Piece selection grid
        piece_selection_layout = QGridLayout()
        self.piece_buttons = {}

        colors = [Color("White"), Color("Black")]
        piece_types = [
            Piecetype("Pawn"),
            Piecetype("Knight"),
            Piecetype("Bishop"),
            Piecetype("Rook"),
            Piecetype("Queen"),
            Piecetype("King"),
        ]

        for col, color in enumerate(colors):
            for row, type in enumerate(piece_types):
                label = ClickablePieceLabel(color, type)
                label.clicked.connect(self.select_piece)
                piece_selection_layout.addWidget(label, row, col)
                self.piece_buttons[label.piece] = label

        layout.addLayout(piece_selection_layout)

        # Validate button
        self.validate_button = QPushButton("Validate")
        self.validate_button.clicked.connect(self.try_validate)
        layout.addWidget(self.validate_button)

        self.setLayout(layout)

        self.piece_to_add = None

        self.pieceToAdd_signal.connect(
            lambda color, piecetype: self.gui.set_piece_to_add(color, piecetype)
        )

        self.boardCleared_signal.connect(self.gui.clear_board)
        self.tryValidate_signal.connect(self.gui.try_validate)

    def clear_board(self):
        self.boardCleared_signal.emit()

    def try_validate(self):
        self.tryValidate_signal.emit()

    def select_piece(self, piece):
        if self.piece_to_add:
            old_piece_label = self.piece_buttons[self.piece_to_add]
            old_piece_label.setStyleSheet("")
        self.piece_to_add = piece
        piece_label = self.piece_buttons[self.piece_to_add]
        piece_label.setStyleSheet("border: 2px solid red;")
        self.pieceToAdd_signal.emit(piece.color, piece.piece_type)
