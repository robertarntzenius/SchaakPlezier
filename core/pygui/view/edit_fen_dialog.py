from PyQt5.QtWidgets import QVBoxLayout, QLineEdit, QDialog, QDialogButtonBox, QLabel

class FenInputDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
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
        layout.addWidget(button_box)

        self.setLayout(layout)

    def get_fen_string(self):
        return self.fen_edit.text()
