# from .observer import ObserverWidget
# from PyQt5.QtWidgets import QWidget, QVBoxLayout, QLabel, QTextEdit
# from chessboard import Chessboard


# class AnalysisBox(Observer, QWidget):
#     def __init__(self, board: Chessboard, parent=None):
#         super().__init__(board, parent=parent)
#         self.layout = QVBoxLayout()

#         history_label = QLabel("Game history")
#         self.layout.addWidget(history_label)

#         self.textbox = QTextEdit()
#         self.layout.addWidget(self.textbox)

#     def notify(self):
#         self.update()
    
#     def update(self):
#         self.repaint()
