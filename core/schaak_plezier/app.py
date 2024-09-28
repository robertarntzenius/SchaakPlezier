from typing import List

from PyQt5.QtWidgets import QApplication

from schaak_plezier.config import Settings
from schaak_plezier.gui import Gui
from schaak_plezier.log import SchaakPlezierLogging


class SchaakPlezier(QApplication):
    def __init__(self, argv: List[str]) -> None:
        super().__init__(argv)

        settings = Settings()
        SchaakPlezierLogging(settings.log)

        self.gui = Gui(settings=settings, parent=None)

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
