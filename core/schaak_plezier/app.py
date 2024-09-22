from typing import List

from wrappers import PlayerType

from schaak_plezier.config import AppConfig
from schaak_plezier.gui import Gui
from schaak_plezier.interface.app import IApplication
from schaak_plezier.log import SchaakPlezierLogging
from schaak_plezier.objects.chessboard import Chessboard


class SchaakPlezier(IApplication):
    def __init__(self, argv: List[str]) -> None:
        super().__init__(argv)

        self.settings = AppConfig()

        SchaakPlezierLogging(self.settings.log)
        self.logger = SchaakPlezierLogging.getLogger(__name__)

        self.board = Chessboard(self.settings.board.fen_string, self.settings.board.log_file)
        self.gui = Gui(app=self, settings=self.settings.gui, parent=None)

        self.gui.playersChanged.connect(lambda white, black: self.set_players(white, black))
        self.gui.doMove.connect(lambda move: self.board.do_move(move))
        self.gui.undoMove.connect(self.board.undo_move)
        self.gui.redoMove.connect(self.board.redo_move)
        self.gui.addPiece.connect(lambda piece: self.board.add_piece(piece))
        self.gui.clearBoard.connect(self.board.clear_board)

        # Create players after gui is created as HumanPlayer needs a ref to the chessboard_view
        self.set_players(self.settings.board.white_player, self.settings.board.black_player)

        self.run()

    def run(self):
        if self.exec_() != 0:
            raise RuntimeError("Application exited with a non-zero exit code.")

    def set_players(self, white: PlayerType, black: PlayerType) -> None:
        self.white_player, self.black_player = self.gui.create_players(white, black)

    # def determine_sound(self, move: Move) -> Sound:
    #     if self.board.in_check:
    #         return Sound.check
    #     elif move.isCastling:
    #         return Sound.castle
    #     elif move.isCapture:
    #         return Sound.capture
    #     else:
    #         if self.board.active_player == Color.White:
    #             return Sound.normal_white
    #         else:
    #             return Sound.normal_black
