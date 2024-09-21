from enum import Enum

from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (
    QAction,
    QDialog,
    QGridLayout,
    QLabel,
    QMainWindow,
    QMessageBox,
    QPushButton,
    QWidget,
)
from wrappers import (
    Board,
    Color,
    GameResult,
    Move,
    Piecetype,
    PlayerType,
    Square,
    makePlayer,
)

from schaak_plezier.config import SETTINGS
from schaak_plezier.interface.app import IApplication
from schaak_plezier.interface.log import SchaakPlezierLogging
from schaak_plezier.interface.player import IPlayer
from schaak_plezier.objects.player import HumanPlayer
from schaak_plezier.widgets.chessboard_view import ChessboardView
from schaak_plezier.widgets.edit_board_dialog import EditBoardDialog
from schaak_plezier.widgets.edit_fen_dialog import EditFenDialog
from schaak_plezier.widgets.edit_players_dialog import EditPlayersDialog
from schaak_plezier.widgets.errordialog import ErrorDialog
from schaak_plezier.widgets.history_box import HistoryBox
from schaak_plezier.widgets.piece import Piece


class Mode(str, Enum):
    IDLE = "IDLE"
    PLAYING = "PLAYING"
    EDIT = "EDIT"


class Gui(QMainWindow):
    def __init__(self, app: IApplication, parent=None):
        super().__init__(parent)
        self.app = app
        self.logger = SchaakPlezierLogging.getLogger(__name__)
        self._mode = Mode.IDLE

    @property
    def mode(self) -> Mode:
        return self._mode

    @mode.setter
    def mode(self, mode: Mode):
        self._mode = mode
        self.chessboard_view.mode = mode

    def build(self):
        self.setWindowTitle(SETTINGS.title)
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QGridLayout(central_widget)

        # OBSERVERS
        self.history_box = HistoryBox(self.app.board, self)

        self.chessboard_view = ChessboardView(self.app.board, self)
        self.chessboard_view.squareClickedInEditMode.connect(
            lambda square: self.try_add_piece(square)
        )

        self.edit_board_dialog = EditBoardDialog(parent=self)
        self.edit_board_dialog.pieceToAdd_signal.connect(lambda piece: self.set_piece_to_add(piece))
        self.edit_board_dialog.hide()

        # INIT BOARD HERE?
        self.set_players(SETTINGS.white_player, SETTINGS.black_player)
        self.initialize_from_fen(SETTINGS.fen_string)

        main_layout.addWidget(self.history_box, 0, 0, 3, 1)
        main_layout.addWidget(self.chessboard_view, 0, 1, 5, 5)
        main_layout.addWidget(self.edit_board_dialog, 0, 6, 5, 1)

        # BUTTONS
        start_button = QPushButton("Start Game")
        start_button.clicked.connect(self.start_game)

        edit_players_button = QPushButton("Edit Players")
        edit_players_button.clicked.connect(self.show_edit_players_dialog)

        edit_fen_button = QPushButton("Edit FEN")
        edit_fen_button.clicked.connect(self.show_edit_fen_dialog)

        edit_board_button = QPushButton("Edit Board")
        edit_board_button.clicked.connect(self.toggle_edit_mode)

        undo_board_button = QPushButton("Undo")
        undo_board_button.clicked.connect(self.undo_move)

        redo_board_button = QPushButton("Redo")
        redo_board_button.clicked.connect(self.redo_move)

        quit_button = QPushButton("Resign")
        quit_button.clicked.connect(self.resign)

        main_layout.addWidget(start_button, 5, 1)
        main_layout.addWidget(edit_fen_button, 5, 2)
        main_layout.addWidget(edit_players_button, 5, 3)
        main_layout.addWidget(edit_board_button, 5, 4)
        main_layout.addWidget(undo_board_button, 5, 5)
        main_layout.addWidget(redo_board_button, 5, 6)
        main_layout.addWidget(quit_button, 5, 7)

        # Menu
        if menu_bar := self.menuBar():
            if file_menu := menu_bar.addMenu("File"):
                exit_action = QAction("Exit", self)
                exit_action.triggered.connect(self.closeEvent)
                file_menu.addAction(exit_action)

        # Display player types
        self.white_player_label = QLabel("White Player: " + SETTINGS.white_player.name)
        self.black_player_label = QLabel("Black Player: " + SETTINGS.black_player.name)

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

    def start_game(self):
        if self.mode != Mode.IDLE:
            self.logger.warning(f"Can only start a game when in IDLE mode. {self.mode}")
            return

        self.logger.debug(
            f"Starting game with players: {self.app.white_player} and {self.app.black_player}"
        )
        self.mode = Mode.PLAYING
        # self.notify_observers(sound=Sound.game_start)

        while (self.app.board.game_result == GameResult.NOT_OVER) and (self.mode == Mode.PLAYING):
            player = self.get_current_player()
            board = self.app.board._board
            player_move = player.decideOnMove(board, board.getPossibleMoves())
            self.do_move(player_move)
            self.logger.debug(f"{player}: {player_move}")

        self.logger.debug(f"Game over: {self.app.board.game_result}")
        # self.notify_observers(sound=Sound.game_end)
        self.mode = Mode.IDLE

    def get_current_player(self) -> IPlayer:
        return (
            self.app.white_player
            if self.app.board.active_player == Color.White
            else self.app.black_player
        )

    def decide_on_move(self, player: IPlayer, board: Board) -> Move:
        return player.decideOnMove(board, board.getPossibleMoves())

    def set_players(self, white_player: PlayerType, black_player: PlayerType):
        self.app.white_player = (
            makePlayer(white_player.name)
            if white_player != PlayerType.Human
            else HumanPlayer(self.chessboard_view)
        )
        self.app.black_player = (
            makePlayer(black_player.name)
            if black_player != PlayerType.Human
            else HumanPlayer(self.chessboard_view)
        )

    def initialize_from_fen(self, fen_string: str) -> None:
        self.app.board.initialize_from_fen(fen_string)

    def do_move(self, move: Move):
        self.app.board.do_move(move)
        # self.notify_observers(sound=self.determine_sound(move))

    def undo_move(self):
        # move = self.app.board.history[-1]
        # self.notify_observers(sound=self.determine_sound(move))
        self.app.board.undo_move()

    def redo_move(self):
        return self.app.board.redo_move()

    def try_add_piece(self, square: Square):
        # signal from chessboardboard_view
        if self.piece_to_add is not None:
            color: Color = self.piece_to_add.color
            type: Piecetype = self.piece_to_add.piece_type
            self.app.board.add_piece(color, type, square)

    def set_piece_to_add(self, piece: Piece):
        # signal from edit board dialog
        self.piece_to_add = piece

    ##### BUTTON CALLBACKS #####
    def clear_board(self):
        self.app.board.clear_board()

    def try_validate(self):
        valid, error_list = self.app.board.validate()

        if valid:
            self.logger.info("Board validated")
            self.toggle_edit_mode()
        else:
            self.logger.warning("\n ".join(error_list))

    def resign(self):
        if self.mode == Mode.PLAYING:
            self.mode = Mode.IDLE
            # self.notify_observers(sound=Sound.game_end)
        self.selected_square = None
        self.selected_piece_moves = []
        if self.mode == Mode.PLAYING:
            QMessageBox.information(self, "Game Over", f"{self.app.board.active_player} resigned.")
        self.update()

    def show_edit_fen_dialog(self):
        if self.mode != Mode.IDLE:
            error_dialog = ErrorDialog(f"Can only edit the fen in IDLE mode: {self.mode}", self)
            error_dialog.exec_()
            return
        while True:
            dialog = EditFenDialog(self)
            if dialog.exec_() == QDialog.Accepted:
                fen_string = dialog.get_fen_string()
                try:
                    self.initialize_from_fen(fen_string)
                    break
                except ValueError as e:
                    error_dialog = ErrorDialog(str(e), self)
                    error_dialog.exec_()
            else:  # Cancel
                break

    def show_edit_players_dialog(self):
        if self.mode != Mode.IDLE:
            error_dialog = ErrorDialog(f"Can only edit the players in IDLE mode: {self.mode}", self)
            error_dialog.exec_()
            return
        while True:
            dialog = EditPlayersDialog(self)
            if dialog.exec_() == QDialog.Accepted:
                white_player, black_player = dialog.get_player_types()
                self.set_players(white_player, black_player)
                self.white_player_label.setText("White Player: " + white_player.name)
                self.black_player_label.setText("Black Player: " + black_player.name)
                break
            else:  # Cancel
                break

    def closeEvent(self, event, *args, **kwargs):
        self.mode = Mode.IDLE
        self.logger.info("Closing application...")
        event.accept()  # Accept the event and close the window
