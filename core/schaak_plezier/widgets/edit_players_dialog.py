from typing import Optional

from PyQt5.QtWidgets import QComboBox, QDialog, QDialogButtonBox, QLabel, QVBoxLayout, QWidget
from wrappers import PlayerType


class EditPlayersDialog(QDialog):
    def __init__(self, parent: Optional[QWidget]):
        super().__init__(parent=parent)
        self.setWindowTitle("Edit players")

        layout = QVBoxLayout()

        player1_label = QLabel("White player:")
        self.player1_combo = QComboBox()
        self.player1_combo.addItems(PlayerType.__members__.keys())

        player2_label = QLabel("Black player:")
        self.player2_combo = QComboBox()
        self.player2_combo.addItems(PlayerType.__members__.keys())

        layout.addWidget(player1_label)
        layout.addWidget(self.player1_combo)
        layout.addWidget(player2_label)
        layout.addWidget(self.player2_combo)

        # Dialog buttons
        buttons = QDialogButtonBox.Ok | QDialogButtonBox.Cancel
        button_box = QDialogButtonBox(buttons)
        button_box.accepted.connect(self.accept)
        button_box.rejected.connect(self.reject)
        layout.addWidget(button_box)

        self.setLayout(layout)

    def get_player_types(self) -> tuple[PlayerType, PlayerType]:
        white = self.player1_combo.currentText()
        black = self.player2_combo.currentText()

        return PlayerType.__members__[white], PlayerType.__members__[black]
