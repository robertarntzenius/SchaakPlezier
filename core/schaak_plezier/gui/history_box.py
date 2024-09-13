from PyQt5.QtCore import QStringListModel
from PyQt5.QtWidgets import QLabel, QListView, QSizePolicy, QVBoxLayout

from schaak_plezier.interface.game import IChessboard
from schaak_plezier.interface.observe import ObserverWidget


class HistoryBox(ObserverWidget):
    def __init__(self, board: IChessboard, parent=None):
        super().__init__(observable_list=[board], parent=parent)
        self.layout = QVBoxLayout()

        history_label = QLabel("Game history")
        self.layout.addWidget(history_label)

        self.list_view = QListView()
        self.model = QStringListModel()
        self.list_view.setModel(self.model)
        self.layout.addWidget(self.list_view)
        self.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

        self.setLayout(self.layout)

    def notify(self, board: IChessboard = None, **kwargs):
        if board is not None:
            self.update_history_list(board)

    def update_history_list(self, board: IChessboard):
        self.model.setStringList([])
        if board.history:
            history_list = [f"{i + 1}. {move}" for i, move in enumerate(board.history)]
            self.model.setStringList(history_list)
