from typing import Optional

from pydantic import ValidationError
from PyQt5.QtCore import Qt, pyqtSignal
from PyQt5.QtWidgets import (
    QAction,
    QDialog,
    QGridLayout,
    QLabel,
    QMainWindow,
    QMenuBar,
    QPushButton,
    QWidget,
)
from wrappers import (
    Color,
    GameResult,
    Move,
    PlayerType,
    Square,
)

from pygui.config import Mode, Settings
from pygui.log import LoggerType, SchaakPlezierLogging
from pygui.objects.chessboard import Chessboard
from pygui.objects.player import CppPlayer, HumanPlayer, IPlayer
from pygui.widgets.chessboard_view import ChessboardView
from pygui.widgets.edit_board_dialog import EditBoardDialog
from pygui.widgets.edit_fen_dialog import EditFenDialog
from pygui.widgets.edit_players_dialog import EditPlayersDialog
from pygui.widgets.history_box import HistoryBox
from pygui.widgets.piece import Piece


class Gui(QMainWindow):
    modeChanged = pyqtSignal(Mode)
    playersChanged = pyqtSignal(PlayerType, PlayerType)

    doMove = pyqtSignal(Move)
    undoMove = pyqtSignal()
    redoMove = pyqtSignal()
    addPiece = pyqtSignal(Piece)
    clearBoard = pyqtSignal()
    tryValidate = pyqtSignal(Chessboard)
    resign = pyqtSignal()

    _mode: Mode = Mode.IDLE

    settings: Settings
    white_player: IPlayer
    black_player: IPlayer
    board: Chessboard

    piece_to_add: Optional[Piece] = None

    def __init__(self, settings: Settings = Settings(), parent: Optional[QWidget] = None):
        super().__init__(parent)
        self.logger = SchaakPlezierLogging.getLogger(
            __name__, level=settings.log.log_level_dialog, type=LoggerType.DIALOG
        )
        self.settings = settings

        self.board = Chessboard(self.settings.board.fen_string, self.settings.board.log_file)
        self.init_UI()
        self.set_players(self.settings.board.white_player, self.settings.board.black_player)

        # SIGNALS
        self.playersChanged.connect(lambda white, black: self.set_players(white, black))
        self.doMove.connect(lambda move: self.board.do_move(move))
        self.undoMove.connect(self.board.undo_move)
        self.redoMove.connect(self.board.redo_move)
        self.addPiece.connect(lambda piece: self.board.add_piece(piece))
        self.clearBoard.connect(self.board.clear_board)
        self.resign.connect(lambda: self.resign_active_player(self.board))

    def init_UI(self):
        self.setWindowTitle(self.settings.gui.title)
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QGridLayout(central_widget)

        # WIDGETS
        self.history_box = HistoryBox(self.board, self)

        self.chessboard_view = ChessboardView(self.board, self.settings.gui.colors, self)
        self.modeChanged.connect(lambda mode: self.chessboard_view.set_mode(mode))
        self.chessboard_view.squareClickedInEditMode.connect(
            lambda square: self.try_add_piece(square)
        )

        self.edit_board_dialog = EditBoardDialog(parent=self)
        self.edit_board_dialog.PieceToAddChanged.connect(lambda piece: self.set_piece_to_add(piece))
        self.edit_board_dialog.TryValidatePressed.connect(lambda: self.try_validate(self.board))
        self.edit_board_dialog.BoardClearPressed.connect(lambda: self.clearBoard.emit())

        self.edit_board_dialog.hide()

        main_layout.addWidget(self.history_box, 0, 0, 3, 1)
        main_layout.addWidget(self.chessboard_view, 0, 1, 5, 5)
        main_layout.addWidget(self.edit_board_dialog, 0, 6, 5, 1)

        # BUTTONS
        start_button = QPushButton("Start Game")
        start_button.clicked.connect(
            lambda: self.start_game(self.board, self.white_player, self.black_player)
        )

        edit_players_button = QPushButton("Edit Players")
        edit_players_button.clicked.connect(self.show_edit_players_dialog)

        edit_fen_button = QPushButton("Edit FEN")
        edit_fen_button.clicked.connect(lambda board: self.show_edit_fen_dialog(self.board))

        edit_board_button = QPushButton("Edit Board")
        edit_board_button.clicked.connect(self.toggle_edit_mode)

        undo_board_button = QPushButton("Undo")
        undo_board_button.clicked.connect(lambda: self.undoMove.emit())

        redo_board_button = QPushButton("Redo")
        redo_board_button.clicked.connect(lambda: self.redoMove.emit())

        quit_button = QPushButton("Resign")
        quit_button.clicked.connect(lambda: self.resign.emit())

        main_layout.addWidget(start_button, 5, 1)
        main_layout.addWidget(edit_fen_button, 5, 2)
        main_layout.addWidget(edit_players_button, 5, 3)
        main_layout.addWidget(edit_board_button, 5, 4)
        main_layout.addWidget(undo_board_button, 5, 5)
        main_layout.addWidget(redo_board_button, 5, 6)
        main_layout.addWidget(quit_button, 5, 7)

        # Menu
        menu_bar = QMenuBar(self)
        if file_menu := menu_bar.addMenu("File"):
            exit_action = QAction("Exit", self)
            exit_action.triggered.connect(self.closeEvent)
            file_menu.addAction(exit_action)
        self.setMenuBar(menu_bar)

        # Display player types
        self.white_player_label = QLabel("White Player: ")
        self.black_player_label = QLabel("Black Player: ")

        main_layout.addWidget(self.black_player_label, 0, 6, alignment=Qt.AlignTop | Qt.AlignRight)
        main_layout.addWidget(
            self.white_player_label, 4, 6, alignment=Qt.AlignBottom | Qt.AlignRight
        )

        self.show()
        self.logger.debug("Built GUI")

    @property
    def mode(self) -> Mode:
        return self._mode

    @mode.setter
    def mode(self, mode: Mode):
        if mode != self._mode:
            self._mode = mode
            self.modeChanged.emit(mode)

    ##### EDIT MODE CALLBACKS #####
    def toggle_edit_mode(self):
        if self.mode == Mode.EDIT:
            self.mode = Mode.IDLE
            self.edit_board_dialog.hide()
        else:
            self.mode = Mode.EDIT
            self.edit_board_dialog.show()

    def start_game(self, board: Chessboard, white_player: IPlayer, black_player: IPlayer):
        if self.mode != Mode.IDLE:
            self.logger.error(f"Can only start a game when in IDLE mode. {self.mode}")
            return

        self.logger.info(
            f"Starting game with players: {white_player.getPlayerType().name} and {black_player.getPlayerType().name}"
        )
        self.mode = Mode.PLAYING
        # self.notify_observers(sound=Sound.game_start)

        while (board.game_result == GameResult.NOT_OVER) and (self.mode == Mode.PLAYING):
            if board.active_player == Color.White:
                player = white_player
            else:
                player = black_player

            player_move = player.decideOnMove(board._board)
            self.doMove.emit(player_move)
            self.logger.debug(f"{player}: {player_move}")

        self.logger.info(f"Game over: {board.game_result}")
        # self.notify_observers(sound=Sound.game_end)
        self.mode = Mode.IDLE

    def set_players(self, white_player: PlayerType, black_player: PlayerType) -> None:
        self.white_player = (
            CppPlayer(white_player)
            if white_player != PlayerType.Human
            else HumanPlayer(self.chessboard_view)
        )
        self.black_player = (
            CppPlayer(black_player)
            if black_player != PlayerType.Human
            else HumanPlayer(self.chessboard_view)
        )
        self.white_player_label.setText("White Player: " + white_player.name)
        self.black_player_label.setText("Black Player: " + black_player.name)

    def do_move(self, move: Move):
        self.doMove.emit(move)

    def undo_move(self):
        self.undoMove.emit()

    def redo_move(self):
        self.redoMove.emit()

    def try_add_piece(self, square: Square):
        # signal from chessboardboard_view
        if self.mode != Mode.EDIT:
            return
        if self.piece_to_add is not None:
            piece = Piece(square, self.piece_to_add.piece_type, self.piece_to_add.color)
            self.addPiece.emit(piece)

    def set_piece_to_add(self, piece: Piece):
        # signal from edit board dialog
        self.piece_to_add = piece

    ##### BUTTON CALLBACKS #####
    def try_validate(self, board: Chessboard):
        valid, error_list = board.validate()

        if valid:
            self.logger.info("Board validated")
            self.toggle_edit_mode()
        else:
            self.logger.error("Could not validate board:\n ".join(error_list))

    def resign_active_player(self, board: Chessboard):
        self.selected_square = None
        if self.mode == Mode.PLAYING:
            self.mode = Mode.IDLE
            self.logger.info(f"{board.active_player.name} resigned.")
        self.update()

    def show_edit_fen_dialog(self, board: Chessboard):
        if self.mode != Mode.IDLE:
            self.logger.error(f"Can only edit the fen in IDLE mode: {self.mode}")
            return
        while True:
            dialog = EditFenDialog(self)
            if dialog.exec_() == QDialog.Accepted:
                try:
                    board.initialize_from_fen(dialog.get_fen_string())
                    break
                except ValidationError as e:
                    self.logger.error(e)
            else:  # Cancel
                break

    def show_edit_players_dialog(self) -> None:
        if self.mode != Mode.IDLE:
            self.logger.error(f"Can only edit the players in IDLE mode: {self.mode}")
            return
        while True:
            dialog = EditPlayersDialog(self)
            if dialog.exec_() == QDialog.Accepted:
                white_player, black_player = dialog.get_player_types()
                self.white_player_label.setText("White Player: " + white_player.name)
                self.black_player_label.setText("Black Player: " + black_player.name)
                self.playersChanged.emit(white_player, black_player)
                break
            else:  # Cancel
                break

    def closeEvent(self, event, *args, **kwargs):
        self.mode = Mode.IDLE
        self.logger.debug("Closing application...")
        event.accept()  # Accept the event and close the window
