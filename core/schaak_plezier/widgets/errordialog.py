from typing import Optional

from PyQt5.QtWidgets import QDialog, QLabel, QPushButton, QVBoxLayout, QWidget


class ErrorDialog(QDialog):
    def __init__(self, error_message: str, parent: Optional[QWidget]):
        super().__init__(parent=parent)
        self.setWindowTitle("Error")

        layout = QVBoxLayout()
        error_label = QLabel(error_message)
        layout.addWidget(error_label)

        ok_button = QPushButton("OK")
        ok_button.clicked.connect(self.accept)
        layout.addWidget(ok_button)

        self.setLayout(layout)
