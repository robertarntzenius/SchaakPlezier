from enum import Enum

from schaak_plezier.controller.sound_player import Sound
from schaak_plezier.interface.app import IController
from schaak_plezier.interface.log import SchaakPlezierLogging
from schaak_plezier.interface.wrapper_types import Color, GameResult, Move, Piecetype, Square
from schaak_plezier.model.chessboard import Chessboard
from schaak_plezier.model.piece import Piece
from schaak_plezier.model.player import HumanPlayer, IPlayer, Player
from schaak_plezier.view.view import View


class Mode(str, Enum):
    IDLE = "IDLE"
    PLAYING = "PLAYING"
    EDIT = "EDIT"


class Controller(IController):
    def __init__(self, config: dict):
        super().__init__()
        self.config = config
        self.logger = SchaakPlezierLogging.getLogger(__name__)
        self.board: Chessboard = Chessboard(config)
        self.view: View = View(self, config)

        self.white_player: IPlayer = None
        self.black_player: IPlayer = None

        self.set_players(self.config.defaults.white_player, self.config.defaults.black_player)
        self.initialize_from_fen(self.config.defaults.fen_string)
        self.mode: Mode = Mode.IDLE

        # SIGNALS
        self.piece_to_add = None
        self.view.chessboard_view.squareClickedInEditMode.connect(
            lambda square: self.try_add_piece(square)
        )
        self.view.edit_board_dialog.pieceToAdd_signal.connect(
            lambda color, piecetype: self.set_piece_to_add(color, piecetype)
        )

        self.view.edit_board_dialog.boardCleared_signal.connect(self.board.clear_board)
        self.view.edit_board_dialog.tryValidate_signal.connect(self.try_validate)

        # self.sound_player = SoundPlayer(self)
        self.logger.info("Created controller")

    def start_game(self) -> GameResult:
        if self.mode != Mode.IDLE:
            self.logger.warning(f"Can only start a game when in IDLE mode. {self.mode}")
            return

        self.logger.debug(
            f"Starting game with players: {self.white_player.player_type} and {self.black_player.player_type}"
        )
        self.mode = Mode.PLAYING
        # self.notify_observers(sound=Sound.game_start)

        while (self.board.game_result == GameResult("NOT_OVER")) and (self.mode == Mode.PLAYING):
            current_player = self.get_current_player()
            player_move = current_player.decide_on_move(self.board)
            self.do_move(player_move)
            self.logger.debug(f"{current_player.player_type}: {player_move}")

        self.logger.debug(f"Game over: {self.board.game_result}")
        # self.notify_observers(sound=Sound.game_end)
        self.mode = Mode.IDLE

        return self.board.game_result

    def get_current_player(self) -> IPlayer:
        return (
            self.white_player if self.board.active_player == Color("White") else self.black_player
        )

    def set_players(self, white: str, black: str) -> None:
        self.white_player: IPlayer = (
            Player(white) if white.lower() != "human" else HumanPlayer(self.view.chessboard_view)
        )
        self.black_player: IPlayer = (
            Player(black) if black.lower() != "human" else HumanPlayer(self.view.chessboard_view)
        )

    def resign(self) -> Color:
        if self.mode == Mode.PLAYING:
            self.mode = Mode.IDLE
            # self.notify_observers(sound=Sound.game_end)
        return self.board.active_player

    def initialize_from_fen(self, fen_string: str) -> None:
        self.board.initialize_from_fen(fen_string)

    def do_move(self, move: Move):
        self.board.do_move(move)
        # self.notify_observers(sound=self.determine_sound(move))

    def undo_move(self):
        # move = self.board.history[-1]
        # self.notify_observers(sound=self.determine_sound(move))
        self.board.undo_move()

    def try_validate(self):
        valid, error_list = self.board.validate()

        if valid:
            self.logger.info("Board validated")
            self.view.toggle_edit_mode()
        else:
            self.logger.warning("\n ".join(error_list))

    def try_add_piece(self, square):
        # signal from chessboardboard_view
        if self.piece_to_add is not None:
            color: Color = self.piece_to_add.color
            type: Piecetype = self.piece_to_add.piece_type
            self.board.add_piece(color, type, square)

    def set_piece_to_add(self, color: Color, type: Piecetype):
        # signal from edit board dialog
        self.piece_to_add = Piece(Square("NoSquare"), type, color)

    def determine_sound(self, move: Move) -> Sound:
        if self.board.in_check:
            return Sound.check
        elif move.isCastling:
            return Sound.castle
        elif move.isCapture:
            return Sound.capture
        else:
            if self.board.active_player == Color("White"):
                return Sound.normal_white
            else:
                return Sound.normal_black
