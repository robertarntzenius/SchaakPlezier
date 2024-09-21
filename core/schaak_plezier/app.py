from typing import List

from schaak_plezier.gui import Gui
from schaak_plezier.interface.app import IApplication
from schaak_plezier.interface.log import SchaakPlezierLogging
from schaak_plezier.objects.chessboard import Chessboard


class SchaakPlezier(IApplication):
    def __init__(self, argv: List[str]) -> None:
        super().__init__(argv)

        SchaakPlezierLogging()  # Init
        self.logger = SchaakPlezierLogging.getLogger(__name__)

        self.board = Chessboard()

        self.gui = Gui(self)
        self.gui.build()

        self.run()

    def run(self):
        if self.exec_() != 0:
            raise RuntimeError("Application exited with a non-zero exit code.")

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
