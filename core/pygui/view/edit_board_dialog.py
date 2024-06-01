from PyQt5.QtWidgets import QVBoxLayout, QPushButton, QGridLayout, QDialog, QLabel, QMessageBox
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import pyqtSignal
from PyQt5 import QtCore

from core.pygui.model.piece import Piece
from core.pygui.model.wrapper_types import Color, Square, Piecetype

class EditBoardDialog(QDialog):
    piecePlaced = pyqtSignal(Color, Piecetype, Square)  # Signal emitted when a piece is placed
    boardCleared = pyqtSignal()

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Edit Board")
        
        layout = QVBoxLayout()

        # Buttons for clearing and editing from the current position
        self.clear_button = QPushButton("Clear Board")
        self.clear_button.clicked.connect(self.clear_board)
        self.edit_button = QPushButton("Edit from Current Position")
        self.edit_button.clicked.connect(self.edit_from_current_position)
        layout.addWidget(self.clear_button)
        layout.addWidget(self.edit_button)

        # Piece selection grid
        piece_selection_layout = QGridLayout()
        self.piece_buttons = {}
        
        colors = [Color('White'), Color('Black')]
        piece_types = [Piecetype('Pawn'), Piecetype('Knight'), Piecetype('Bishop'), Piecetype('Rook'), Piecetype('Queen'), Piecetype('King')]
        for col, color in enumerate(colors):
            for row, type in enumerate(piece_types):
                label = QLabel()
                piece = Piece(Square('NoSquare'), type, color)
                pixmap = QPixmap.fromImage(piece.image).scaled(50, 50)
                label.setPixmap(pixmap)
                label.setAlignment(QtCore.Qt.AlignCenter)
                piece_selection_layout.addWidget(label, row, col)
                self.piece_buttons[piece] = label

        layout.addLayout(piece_selection_layout)

        # Validate button
        self.validate_button = QPushButton("Validate")
        self.validate_button.clicked.connect(self.validate_position)
        layout.addWidget(self.validate_button)

        self.setLayout(layout)
        
        self.selected_piece = None

    def clear_board(self):
        self.boardCleared.emit()
        print("Board cleared")

    def edit_from_current_position(self):
        print("Editing from current position")

    def validate_position(self):
        self.try_validate.emit()
        valid = True  # Replace with actual validation logic
        if valid:
            self.accept()
        else:
            QMessageBox.warning(self, "Invalid Position", "The position is not valid.")

    def select_piece(self, piece):
        # Highlight selected piece
        if self.selected_piece:
            old_piece_label = self.piece_buttons[self.selected_piece]
            old_piece_label.setStyleSheet("")  # Reset style sheet
        self.selected_piece = piece
        piece_label = self.piece_buttons[self.selected_piece]
        piece_label.setStyleSheet("border: 2px solid red;")