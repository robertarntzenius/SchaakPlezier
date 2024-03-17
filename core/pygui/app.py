from PyQt5.QtWidgets import QMainWindow, QWidget, QGridLayout, QPushButton, QLabel, QTextEdit, QAction, QSizePolicy
from PyQt5.QtGui import QIcon, QPixmap, QPainter, QColor
from PyQt5.QtCore import Qt

from chessboard import ChessBoard


class SchaakPlezierApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('SchaakPlezier')

        self.initUI()

    def initUI(self):
        central_widget = QWidget()
        self.setCentralWidget(central_widget)

        # Create layout
        main_layout = QGridLayout(central_widget)

        # Create left panel with moves and evaluation
        moves_label = QLabel("Moves and Evaluation")
        # moves_label.setAlignment(Qt.AlignCenter)
        main_layout.addWidget(moves_label, 0, 0, 1, 5)

        moves_text_edit = QTextEdit()
        main_layout.addWidget(moves_text_edit, 1, 0, 4, 1)

        # Create chessboard in the middle
        
        chessboard = ChessBoard(self)
        chessboard.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        main_layout.addWidget(chessboard, 1, 1, 3, 3)

        # Create buttons on the right
        start_button = QPushButton("Start Game")
        edit_players_button = QPushButton("Edit Players")
        edit_board_button = QPushButton("Edit Board")
        quit_button = QPushButton("Quit")
        main_layout.addWidget(start_button, 1, 4)
        main_layout.addWidget(edit_players_button, 2, 4)
        main_layout.addWidget(edit_board_button, 3, 4)
        main_layout.addWidget(quit_button, 4, 4)

        # Create menu bar
        menu_bar = self.menuBar()
        file_menu = menu_bar.addMenu("File")
        exit_action = QAction("Exit", self)
        exit_action.triggered.connect(self.close)
        file_menu.addAction(exit_action)

        self.show()
