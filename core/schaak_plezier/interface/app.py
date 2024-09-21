from abc import abstractmethod

from PyQt5.QtWidgets import QApplication, QMainWindow
from schaak_plezier.interface.player import IPlayer
from schaak_plezier.objects.chessboard import Chessboard


class IApplication(QApplication):
    """Container for the GUI and the models responsible for connecting the gui's signals to the slots of the models."""

    gui: QMainWindow
    white_player: IPlayer
    black_player: IPlayer
    board: Chessboard

    @abstractmethod
    def build(self): ...

    @abstractmethod
    def run(self): ...
