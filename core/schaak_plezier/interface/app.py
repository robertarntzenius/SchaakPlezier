import logging
from abc import ABC, abstractmethod

from PyQt5.QtWidgets import QApplication, QMainWindow
from schaak_plezier.interface.config import GUIConfig
from schaak_plezier.interface.game import IChessboard, IPlayer


class IController(ABC):
    white_player: IPlayer
    black_player: IPlayer


class ISchaakPlezier(QApplication):
    logger: logging.Logger
    config: GUIConfig
    controller: IController
    board: IChessboard
    view: QMainWindow

    @abstractmethod
    def build(self): ...

    @abstractmethod
    def run(self): ...
