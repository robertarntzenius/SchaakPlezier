import logging
from abc import abstractmethod
from enum import Enum

from PyQt5.QtWidgets import QApplication, QMainWindow
from schaak_plezier.interface.game import IChessboard, IPlayer
from schaak_plezier.interface.observe import ObserverWidget


class Mode(str, Enum):
    IDLE = "IDLE"
    PLAYING = "PLAYING"
    EDIT = "EDIT"


class IGUI(QMainWindow):
    mode: Mode
    chessboard_view: ObserverWidget

    @abstractmethod
    def build(self): ...

    @abstractmethod
    def toggle_edit_mode(): ...


class IApplication(QApplication):
    logger: logging.Logger

    view: IGUI

    white_player: IPlayer
    black_player: IPlayer
    board: IChessboard

    @abstractmethod
    def build(self): ...

    @abstractmethod
    def run(self): ...

    @abstractmethod
    def undo_move(): ...

    @abstractmethod
    def start_game(): ...

    @abstractmethod
    def set_players(white_player: str, black_player: str): ...
