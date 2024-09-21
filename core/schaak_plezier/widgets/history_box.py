from typing import Optional

from PyQt5.QtCore import QStringListModel
from PyQt5.QtWidgets import QLabel, QListView, QSizePolicy, QVBoxLayout, QWidget

from schaak_plezier.interface.observe import ObserverWidget
from schaak_plezier.objects.chessboard import Chessboard


class HistoryBox(ObserverWidget):
    def __init__(self, board: Chessboard, parent: Optional[QWidget]):
        super().__init__(observable=board, parent=parent)
        layout = QVBoxLayout()

        history_label = QLabel("Game history")
        layout.addWidget(history_label)

        self.list_view = QListView()
        self.model = QStringListModel()
        self.list_view.setModel(self.model)
        layout.addWidget(self.list_view)
        self.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

        self.setLayout(layout)

    def notify(self, board: Optional[Chessboard] = None, **kwargs):
        if board is not None:
            self.update_history_list(board)

    def update_history_list(self, board: Chessboard):
        self.model.setStringList([])
        if board.history:
            history_list = [f"{i + 1}. {move}" for i, move in enumerate(board.history)]
            self.model.setStringList(history_list)
