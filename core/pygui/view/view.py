import logging
from PyQt5.QtWidgets import QMainWindow, QWidget, QGridLayout, QPushButton, QAction, QDialog, QMessageBox, QLabel
from PyQt5.QtCore import Qt

from core.pygui.controller.controller import Controller, Mode
from core.pygui.view.edit_board_dialog import EditBoardDialog
from core.pygui.view.edit_fen_dialog import EditFenDialog
from core.pygui.view.edit_players_dialog import EditPlayersDialog
from core.pygui.view.errordialog import ErrorDialog

from .chessboard_view import ChessboardView
from .history_box import HistoryBox


class View(QMainWindow):
    def __init__(self, controller: Controller, config: dict):
        super().__init__()
        self.config = config
        self.controller = controller

        self.setWindowTitle(self.config.gui.title)
        self.initUI()
        self.show()
        logging.info('Created view')

    def initUI(self):
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QGridLayout(central_widget)

        # OBSERVERS
        self.history_box = HistoryBox(self.controller.board, self)
        self.chessboard_view = ChessboardView(self.controller, self.config, self)
        self.edit_board_dialog = EditBoardDialog(self)

        main_layout.addWidget(self.history_box, 0, 0, 3, 1)
        main_layout.addWidget(self.chessboard_view, 0, 1, 5, 5)
        main_layout.addWidget(self.edit_board_dialog, 0, 6, 5, 1)

        # BUTTONS
        start_button = QPushButton("Start Game")
        start_button.clicked.connect(self.start_game)

        edit_players_button = QPushButton("Edit Players")
        edit_players_button.clicked.connect(self.edit_players_dialog)

        edit_fen_button = QPushButton("Edit FEN")
        edit_fen_button.clicked.connect(self.edit_fen_dialog)

        edit_board_button = QPushButton("Edit Board")
        edit_board_button.clicked.connect(self.toggle_edit_mode)
        for piece, label in self.edit_board_dialog.piece_buttons.items():
            label.mousePressEvent = lambda event, piece=piece: self.edit_board_dialog.select_piece(piece)
        self.edit_board_dialog.hide()

        undo_board_button = QPushButton("Undo")
        undo_board_button.clicked.connect(self.controller.undo_move)

        quit_button = QPushButton("Resign")
        quit_button.clicked.connect(self.resign)

        main_layout.addWidget(start_button, 5, 1)
        main_layout.addWidget(edit_fen_button, 5, 2)
        main_layout.addWidget(edit_players_button, 5, 3)
        main_layout.addWidget(edit_board_button, 5, 4)
        main_layout.addWidget(undo_board_button, 5, 5)
        main_layout.addWidget(quit_button, 5, 6)

        # Menu
        menu_bar = self.menuBar()
        file_menu = menu_bar.addMenu("File")
        exit_action = QAction("Exit", self)
        exit_action.triggered.connect(self.close)
        file_menu.addAction(exit_action)
        
        # Display player types
        self.white_player_label = QLabel("White Player: " + self.config.defaults.white_player)
        self.black_player_label = QLabel("Black Player: " + self.config.defaults.black_player)

        main_layout.addWidget(self.black_player_label, 0, 6, alignment=Qt.AlignTop | Qt.AlignRight)
        main_layout.addWidget(self.white_player_label, 4, 6, alignment=Qt.AlignBottom | Qt.AlignRight)

        self.show()
    
    ##### EDIT MODE CALLBACKS #####
    def toggle_edit_mode(self):
        if self.controller.mode == Mode.EDIT:
            self.controller.mode = Mode.IDLE
            self.edit_board_dialog.hide()
        else:
            self.controller.mode = Mode.EDIT
            self.edit_board_dialog.show()


    ##### BUTTON CALLBACKS #####
    def start_game(self):
        self.controller.start_game()

    def resign(self):
        self.selected_square = None
        self.selected_piece_moves = []
        if self.controller.mode == Mode.PLAYING:
            losing_color = self.controller.resign()
            QMessageBox.information(self, "Game Over", f"{losing_color} resigned.")
        self.update()


    def edit_fen_dialog(self):
        if self.controller.mode != Mode.IDLE:
            error_dialog = ErrorDialog(f"Can only edit the fen in IDLE mode: {self.controller.mode}", self)
            error_dialog.exec_()
            return
        while True:
            dialog = EditFenDialog(self)
            if dialog.exec_() == QDialog.Accepted:
                fen_string = dialog.get_fen_string()
                try:
                    self.controller.initialize_from_fen(fen_string)
                    break
                except ValueError as e:
                    error_dialog = ErrorDialog(str(e), self)
                    error_dialog.exec_()
            else: # Cancel
                break
        
    def edit_players_dialog(self):
        if self.controller.mode != Mode.IDLE:
            error_dialog = ErrorDialog(f"Can only edit the players in IDLE mode: {self.controller.mode}", self)
            error_dialog.exec_()
            return
        while True:
            dialog = EditPlayersDialog(self)
            if dialog.exec_() == QDialog.Accepted:
                white_player, black_player = dialog.get_player_types()
                self.controller.set_players(white_player, black_player)
                self.white_player_label.setText("White Player: " + white_player)
                self.black_player_label.setText("Black Player: " + black_player)
                break
            else: # Cancel
                break
    
    def closeEvent(self, event):
        self.controller.mode = Mode.IDLE
        logging.info("Closing application...")
        event.accept() # Accept the event and close the window