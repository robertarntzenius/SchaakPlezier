from model.chessboard import Chessboard
from view.view import View
from model.definitions import *

class Controller():
    def __init__(self, board: Chessboard, view: View):
        super().__init__()
        self.board = board
        self.view = view

        self.players = []

