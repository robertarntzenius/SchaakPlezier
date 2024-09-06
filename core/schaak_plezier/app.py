from typing import List

from schaak_plezier.gui.gui import Gui
from schaak_plezier.interface.app import IApplication, Mode
from schaak_plezier.interface.config import SETTINGS
from schaak_plezier.interface.game import IPlayer
from schaak_plezier.interface.log import FixedWidthFormatter, SchaakPlezierLogging
from schaak_plezier.interface.wrapper_types import Color, GameResult, Move, Piecetype, Square
from schaak_plezier.model.chessboard import Chessboard
from schaak_plezier.model.piece import Piece
from schaak_plezier.model.player import HumanPlayer, Player


class SchaakPlezier(IApplication):
    board: Chessboard
    white_player: IPlayer
    black_player: IPlayer
    piece_to_add: Piece

    gui: Gui

    def __init__(self, argv: List[str]) -> None:
        super().__init__(argv)

        SchaakPlezierLogging(
            file_path=SETTINGS.log_file,
            loglevel_root=SETTINGS.log_level,
            formatter=FixedWidthFormatter(),
        )
        self.logger = SchaakPlezierLogging.getLogger(__name__)

        self.board = Chessboard()

        self.gui = Gui(self)
        self.gui.update()

    def build(self):
        self.set_players(SETTINGS.white_player, SETTINGS.black_player)
        self.initialize_from_fen(SETTINGS.fen_string)
        self.gui.build()

    def run(self):
        if self.exec_() != 0:
            raise RuntimeError("Application exited with a non-zero exit code.")

    def start_game(self) -> GameResult:
        if self.gui.mode != Mode.IDLE:
            self.logger.warning(f"Can only start a game when in IDLE mode. {self.gui.mode}")
            return

        self.logger.debug(
            f"Starting game with players: {self.white_player.player_type} and {self.black_player.player_type}"
        )
        self.gui.mode = Mode.PLAYING
        # self.notify_observers(sound=Sound.game_start)

        while (self.board.game_result == GameResult("NOT_OVER")) and (
            self.gui.mode == Mode.PLAYING
        ):
            current_player = self.get_current_player()
            player_move = current_player.decide_on_move(self.board)
            self.do_move(player_move)
            self.logger.debug(f"{current_player.player_type}: {player_move}")

        self.logger.debug(f"Game over: {self.board.game_result}")
        # self.notify_observers(sound=Sound.game_end)
        self.gui.mode = Mode.IDLE

        return self.board.game_result

    def get_current_player(self) -> IPlayer:
        return (
            self.white_player if self.board.active_player == Color("White") else self.black_player
        )

    def set_players(self, white: str, black: str) -> None:
        self.white_player: IPlayer = (
            Player(white) if white.lower() != "human" else HumanPlayer(self)
        )
        self.black_player: IPlayer = (
            Player(black) if black.lower() != "human" else HumanPlayer(self)
        )

    def resign(self) -> Color:
        if self.gui.mode == Mode.PLAYING:
            self.gui.mode = Mode.IDLE
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

    def redo_move(self):
        return self.board.redo_move()

    def try_validate(self):
        valid, error_list = self.board.validate()

        if valid:
            self.logger.info("Board validated")
            self.gui.toggle_edit_mode()
        else:
            self.logger.warning("\n ".join(error_list))

    def clear_board(self):
        self.board.clear_board()

    def try_add_piece(self, square):
        # signal from chessboardboard_view
        if self.piece_to_add is not None:
            color: Color = self.piece_to_add.color
            type: Piecetype = self.piece_to_add.piece_type
            self.board.add_piece(color, type, square)

    def set_piece_to_add(self, color: Color, type: Piecetype):
        # signal from edit board dialog
        self.piece_to_add = Piece(Square("NoSquare"), type, color)

    # def determine_sound(self, move: Move) -> Sound:
    #     if self.board.in_check:
    #         return Sound.check
    #     elif move.isCastling:
    #         return Sound.castle
    #     elif move.isCapture:
    #         return Sound.capture
    #     else:
    #         if self.board.active_player == Color("White"):
    #             return Sound.normal_white
    #         else:
    #             return Sound.normal_black
