from PyQt5.QtWidgets import QMainWindow, QWidget, QGridLayout, QPushButton, QAction, QSizePolicy

from view import ChessboardView, FenInputDialog, ErrorDialog, HistoryBox

class View(QMainWindow):
    def __init__(self, chessboard):
        super().__init__()

        self.setWindowTitle('SchaakPlezier')
        self.initUI(chessboard)
        self.show()

    def initUI(self, chessboard):
        central_widget = QWidget()
        self.setCentralWidget(central_widget)

        # Create layout
        main_layout = QGridLayout(central_widget)

        # # Create observers
        self.history_box = HistoryBox(chessboard, self)

        main_layout.addWidget(self.history_box, 0, 0, 3, 1)

        self.chessboard_view = ChessboardView(chessboard, self)
        main_layout.addWidget(self.chessboard_view, 0, 1, 5, 5)

        # Create buttons on the bottom
        start_button = QPushButton("Start Game")
        start_button.clicked.connect(self.chessboard_view.start_game)

        edit_players_button = QPushButton("Edit Players")
        edit_players_button.clicked.connect(self.chessboard_view.edit_players)

        edit_fen_button = QPushButton("Edit FEN")
        edit_fen_button.clicked.connect(self.chessboard_view.edit_fen_dialog)

        edit_board_button = QPushButton("Edit Board")
        edit_board_button.clicked.connect(self.chessboard_view.edit_board)

        undo_board_button = QPushButton("Undo")
        undo_board_button.clicked.connect(chessboard.undo_move)

        quit_button = QPushButton("Resign")
        quit_button.clicked.connect(self.chessboard_view.resign)

        main_layout.addWidget(start_button, 5, 1)
        main_layout.addWidget(edit_fen_button, 5, 2)
        main_layout.addWidget(edit_players_button, 5, 3)
        main_layout.addWidget(edit_board_button, 5, 4)
        main_layout.addWidget(undo_board_button, 5, 5)
        main_layout.addWidget(quit_button, 5, 6)

        # Create menu bar
        # menu_bar = self.menuBar()
        # file_menu = menu_bar.addMenu("File")
        # exit_action = QAction("Exit", self)
        # exit_action.triggered.connect(self.close)
        # file_menu.addAction(exit_action)

        self.show()
