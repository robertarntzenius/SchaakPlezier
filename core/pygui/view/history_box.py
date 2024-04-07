from model.chessboard import Chessboard
from model.wrapper_types import Move

from .observer import ObserverWidget

from PyQt5.QtWidgets import QListView, QVBoxLayout, QLabel, QSizePolicy
from PyQt5.QtCore import QStringListModel

class HistoryBox(ObserverWidget):
    def __init__(self, board: Chessboard, parent=None):
        super().__init__(observable=board, parent=parent)
        self.board = board
        self.layout = QVBoxLayout()

        history_label = QLabel("Game history")
        self.layout.addWidget(history_label)

        self.list_view = QListView()
        self.model = QStringListModel()
        self.list_view.setModel(self.model)
        self.layout.addWidget(self.list_view)
        self.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

        self.setLayout(self.layout)

    def notify(self, board = None):
        if board is not None:
            self.update_history_list()

    def update_history_list(self):
        self.model.setStringList([])
        if self.board.history:
            history_list = [f"{i + 1}. {Move(move)}" for i, move in enumerate(self.board.history)]
            self.model.setStringList(history_list)