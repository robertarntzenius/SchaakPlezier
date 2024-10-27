from typing import Optional

from PyQt5.QtCore import QStringListModel
from PyQt5.QtWidgets import QLabel, QListView, QSizePolicy, QVBoxLayout, QWidget

from pygui.objects.chessboard import Chessboard


class HistoryBox(QWidget):
    def __init__(self, board: Chessboard, parent: Optional[QWidget]):
        super().__init__(parent=parent)
        board.boardChanged.connect(lambda _board: self.handle_board_changed(_board))
        layout = QVBoxLayout()

        history_label = QLabel("Game history")
        layout.addWidget(history_label)

        self.list_view = QListView()
        self.model = QStringListModel()
        self.list_view.setModel(self.model)
        layout.addWidget(self.list_view)
        self.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

        self.setLayout(layout)

    def handle_board_changed(self, board: Chessboard):
        self.model.setStringList([])
        history_list = [f"{i + 1}. {move}" for i, move in enumerate(board.history)]
        self.model.setStringList(history_list)
