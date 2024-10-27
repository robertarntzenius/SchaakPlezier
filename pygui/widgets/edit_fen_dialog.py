from typing import Optional

from PyQt5.QtWidgets import (
    QDialog,
    QDialogButtonBox,
    QLabel,
    QLineEdit,
    QPushButton,
    QVBoxLayout,
    QWidget,
)

from pygui.config import FENString


class EditFenDialog(QDialog):
    def __init__(self, parent: Optional[QWidget]):
        super().__init__(parent=parent)
        self.setWindowTitle("Edit FEN")

        layout = QVBoxLayout()
        self.fen_label = QLabel("Enter FEN string:")
        self.fen_edit = QLineEdit()
        layout.addWidget(self.fen_label)
        layout.addWidget(self.fen_edit)

        buttons = QDialogButtonBox.Ok | QDialogButtonBox.Cancel
        button_box = QDialogButtonBox(buttons)
        button_box.accepted.connect(self.accept)
        button_box.rejected.connect(self.reject)

        default_fen_button = QPushButton("Reset board")
        default_fen_button.clicked.connect(self.set_default_fen)

        layout.addWidget(button_box)
        layout.addWidget(default_fen_button)

        self.setLayout(layout)

    def get_fen_string(self) -> str:
        return self.fen_edit.text()

    def set_default_fen(self):
        self.fen_edit.setText(FENString().string)
