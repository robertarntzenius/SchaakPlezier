from PyQt5.QtWidgets import QVBoxLayout, QDialog, QDialogButtonBox, QLabel, QComboBox

class EditPlayersDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Edit players")

        layout = QVBoxLayout()

        player1_label = QLabel("White player:")
        self.player1_combo = QComboBox()
        self.player1_combo.addItems(["Human", "MinMax", "MonteCarlo", "Random", "AlphaBeta"])

        player2_label = QLabel("Black player:")
        self.player2_combo = QComboBox()
        self.player2_combo.addItems(["Human", "MinMax", "MonteCarlo", "Random", "AlphaBeta"])

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

    def get_player_types(self):
        white = self.player1_combo.currentText()
        black = self.player2_combo.currentText()
        return white, black
