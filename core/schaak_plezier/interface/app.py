import logging
from abc import ABC, abstractmethod
from enum import Enum

from PyQt5.QtWidgets import QApplication, QMainWindow
from schaak_plezier.interface.config import GUIConfig
from schaak_plezier.interface.game import IChessboard, IPlayer
from schaak_plezier.interface.observe import ObserverWidget


class Mode(str, Enum):
    IDLE = "IDLE"
    PLAYING = "PLAYING"
    EDIT = "EDIT"


class IController(ABC):
    white_player: IPlayer
    black_player: IPlayer
    board: IChessboard
    mode: Mode

    @abstractmethod
    def undo_move(): ...

    @abstractmethod
    def start_game(): ...

    @abstractmethod
    def set_players(white_player: str, black_player: str): ...


class IView(QMainWindow):
    controller: IController
    chessboard_view: ObserverWidget

    @abstractmethod
    def toggle_edit_mode(): ...


class ISchaakPlezier(QApplication):
    logger: logging.Logger
    config: GUIConfig

    controller: IController
    view: IView

    @abstractmethod
    def build(self): ...

    @abstractmethod
    def run(self): ...
