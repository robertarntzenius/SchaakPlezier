from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (
    QAction,
    QDialog,
    QGridLayout,
    QLabel,
    QMessageBox,
    QPushButton,
    QWidget,
)

from schaak_plezier.gui.chessboard_view import ChessboardView, IChessboardView
from schaak_plezier.gui.edit_board_dialog import EditBoardDialog
from schaak_plezier.gui.edit_fen_dialog import EditFenDialog
from schaak_plezier.gui.edit_players_dialog import EditPlayersDialog
from schaak_plezier.gui.errordialog import ErrorDialog
from schaak_plezier.gui.history_box import HistoryBox
from schaak_plezier.interface.app import IGUI, IApplication, Mode
from schaak_plezier.interface.config import SETTINGS
from schaak_plezier.interface.log import SchaakPlezierLogging


class Gui(IGUI):
    history_box: HistoryBox
    chessboard_view: IChessboardView
    edit_board_dialog: EditBoardDialog

    def __init__(self, app: IApplication):
        super().__init__()
        self.app = app
        self.logger = SchaakPlezierLogging.getLogger(__name__)
        self.mode = Mode.IDLE

        # OBSERVERS
        self.history_box = HistoryBox(self.app.board, self)
        self.chessboard_view = ChessboardView(self.app.board, self)
        self.edit_board_dialog = EditBoardDialog(self)

    def build(self):
        self.setWindowTitle(SETTINGS.title)
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QGridLayout(central_widget)

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
            label.mousePressEvent = lambda event, piece=piece: self.edit_board_dialog.select_piece(
                piece
            )
        self.edit_board_dialog.hide()

        undo_board_button = QPushButton("Undo")
        undo_board_button.clicked.connect(self.app.undo_move)

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
        self.white_player_label = QLabel("White Player: " + SETTINGS.white_player)
        self.black_player_label = QLabel("Black Player: " + SETTINGS.black_player)

        main_layout.addWidget(self.black_player_label, 0, 6, alignment=Qt.AlignTop | Qt.AlignRight)
        main_layout.addWidget(
            self.white_player_label, 4, 6, alignment=Qt.AlignBottom | Qt.AlignRight
        )

        self.show()
        self.logger.info("Built GUI")

    ##### EDIT MODE CALLBACKS #####
    def toggle_edit_mode(self):
        if self.mode == Mode.EDIT:
            self.mode = Mode.IDLE
            self.edit_board_dialog.hide()
        else:
            self.mode = Mode.EDIT
            self.edit_board_dialog.show()

    def try_add_piece(self, square):
        self.app.try_add_piece(square)

    def set_piece_to_add(self, color, piecetype):
        self.app.set_piece_to_add(color, piecetype)

    ##### BUTTON CALLBACKS #####
    def start_game(self):
        self.app.start_game()

    def clear_board(self):
        self.app.clear_board()

    def try_validate(self):
        self.app.try_validate()

    def resign(self):
        self.selected_square = None
        self.selected_piece_moves = []
        if self.mode == Mode.PLAYING:
            losing_color = self.app.resign()
            QMessageBox.information(self, "Game Over", f"{losing_color} resigned.")
        self.update()

    def edit_fen_dialog(self):
        if self.mode != Mode.IDLE:
            error_dialog = ErrorDialog(f"Can only edit the fen in IDLE mode: {self.mode}", self)
            error_dialog.exec_()
            return
        while True:
            dialog = EditFenDialog(self)
            if dialog.exec_() == QDialog.Accepted:
                fen_string = dialog.get_fen_string()
                try:
                    self.app.initialize_from_fen(fen_string)
                    break
                except ValueError as e:
                    error_dialog = ErrorDialog(str(e), self)
                    error_dialog.exec_()
            else:  # Cancel
                break

    def edit_players_dialog(self):
        if self.mode != Mode.IDLE:
            error_dialog = ErrorDialog(f"Can only edit the players in IDLE mode: {self.mode}", self)
            error_dialog.exec_()
            return
        while True:
            dialog = EditPlayersDialog(self)
            if dialog.exec_() == QDialog.Accepted:
                white_player, black_player = dialog.get_player_types()
                self.app.set_players(white_player, black_player)
                self.white_player_label.setText("White Player: " + white_player)
                self.black_player_label.setText("Black Player: " + black_player)
                break
            else:  # Cancel
                break

    def closeEvent(self, event):
        self.mode = Mode.IDLE
        self.logger.info("Closing application...")
        event.accept()  # Accept the event and close the window
