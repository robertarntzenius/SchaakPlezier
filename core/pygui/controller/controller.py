from core.pygui.controller.sound_player import Sound, SoundPlayer
from core.pygui.model.chessboard import Chessboard
from core.pygui.model.observable import Observable
from core.pygui.model.player import HumanPlayer, IPlayer, Player
from core.pygui.model.wrapper_types import Color, GameResult, Move

import logging

import core.pygui.view.view as view

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
        self.playing: bool = False
        
        self.sound_player = SoundPlayer()
        logging.info('Created controller')


    def start_game(self) -> GameResult:    
        logging.debug(f'Starting game with players: {self.white_player.player_type} and {self.black_player.player_type}')
        self.playing = True
        self.sound_player.play(sound=Sound.game_start)

        while self.board.game_result == GameResult('NOT_OVER'):
            current_player = self.get_current_player()
            player_move = current_player.decide_on_move(self.board)
            self.do_move(player_move)
            logging.debug(f"{current_player.player_type}: {player_move}")
        
        logging.debug(f"Game over: {self.board.game_result}")
        self.playing = False
        self.notify_observers(sound=Sound.game_end)

        return self.board.game_result

    def get_current_player(self) -> IPlayer:
        return self.white_player if self.board.active_player == Color('White') else self.black_player

    def set_players(self, white: str, black: str) -> None:
        self.white_player: IPlayer = Player(white) if white.lower() != 'human' else HumanPlayer(self.view.chessboard_view)
        self.black_player: IPlayer = Player(black) if black.lower() != 'human' else HumanPlayer(self.view.chessboard_view)

    def resign(self) -> Color:
        if self.playing:
            self.playing = False
            self.sound_player.play(sound=Sound.game_end)
        return self.board.active_player

    def initialize_from_fen(self, fen_string: str) -> None:
        self.board.initialize_from_fen(fen_string)

    def do_move(self, move: Move):
        self.board.do_move(move)
        self.sound_player.play(sound=self.determine_sound(move))

    def undo_move(self):
        move = self.board.history[-1]
        self.sound_player.play(sound=self.determine_sound(move))
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