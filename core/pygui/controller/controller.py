from enum import Enum
from core.pygui.controller.sound_player import Sound, SoundPlayer
from core.pygui.model.chessboard import Chessboard
from core.pygui.model.observable import Observable
from core.pygui.model.player import HumanPlayer, IPlayer, Player
from core.pygui.model.wrapper_types import Color, GameResult, Move

import logging

import core.pygui.view.view as view

class Mode(str, Enum):
    IDLE = 'IDLE'
    PLAYING = 'PLAYING'
    EDIT = 'EDIT'

class Controller(Observable):
    def __init__(self, config: dict):
        super().__init__()
        self.config = config
        self.board: Chessboard = Chessboard(config)
        self.view: view.View = view.View(self, config)

        self.white_player: IPlayer = None
        self.black_player: IPlayer = None

        self.set_players(self.config.defaults.white_player, self.config.defaults.black_player)
        self.initialize_from_fen(self.config.defaults.fen_string)
        self.mode: Mode = Mode.IDLE
        
        # SIGNALS 
        self.view.edit_board_dialog.piecePlaced.connect(self.board.add_piece)
        self.view.edit_board_dialog.boardCleared.connect(self.board.clear_board)

        self.sound_player = SoundPlayer(self)
        logging.info('Created controller')


    def start_game(self) -> GameResult:
        if self.mode != Mode.IDLE:
            logging.warning(f'Can only start a game when in IDLE mode. {self.mode}')
            return
        
        logging.debug(f'Starting game with players: {self.white_player.player_type} and {self.black_player.player_type}')
        self.mode = Mode.PLAYING
        self.notify_observers(sound=Sound.game_start)

        while self.board.game_result == GameResult('NOT_OVER'):
            current_player = self.get_current_player()
            player_move = current_player.decide_on_move(self.board)
            self.do_move(player_move)
            logging.debug(f"{current_player.player_type}: {player_move}")
        
        logging.debug(f"Game over: {self.board.game_result}")
        self.notify_observers(sound=Sound.game_end)
        self.MODE = Mode.IDLE

        return self.board.game_result

    def get_current_player(self) -> IPlayer:
        return self.white_player if self.board.active_player == Color('White') else self.black_player

    def set_players(self, white: str, black: str) -> None:
        self.white_player: IPlayer = Player(white) if white.lower() != 'human' else HumanPlayer(self.view.chessboard_view)
        self.black_player: IPlayer = Player(black) if black.lower() != 'human' else HumanPlayer(self.view.chessboard_view)

    def resign(self) -> Color:
        if self.mode == Mode.PLAYING:
            self.mode = Mode.IDLE
            self.notify_observers(sound=Sound.game_end)
        return self.board.active_player

    def initialize_from_fen(self, fen_string: str) -> None:
        self.board.initialize_from_fen(fen_string)

    def do_move(self, move: Move):
        self.board.do_move(move)
        self.notify_observers(sound=self.determine_sound(move))

    def undo_move(self):
        move = self.board.history[-1]
        self.notify_observers(sound=self.determine_sound(move))
        self.board.undo_move()

    def determine_sound(self, move: Move) -> Sound:
        if self.board.in_check:
            return Sound.check
        elif move.isCastling:
            return Sound.castle
        elif move.isCapture:
            return Sound.capture
        else:
            if self.board.active_player == Color('White'):
                return Sound.normal_white
            else:
                return Sound.normal_black