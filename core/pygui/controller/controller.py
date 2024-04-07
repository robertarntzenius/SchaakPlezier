from core.pygui.model.chessboard import Chessboard
from core.pygui.model.player import HumanPlayer, IPlayer, Player
from core.pygui.model.wrapper_types import Color, GameResult, Move

import logging

import core.pygui.view.view as view

class Controller():
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

        logging.info('Created controller')    

    def start_game(self) -> GameResult:    
        logging.debug(f'Starting game with players: {self.white_player.player_type} and {self.black_player.player_type}')
        self.playing = True

        while self.board.game_result == GameResult('NOT_OVER'):
            current_player = self.get_current_player()
            player_move = current_player.decide_on_move(self.board)
            self.board.do_move(player_move)
            logging.debug(f"player_choice: {player_move}")
        
        logging.debug(f"Game over: {self.board.game_result}")
        self.playing = False
        return self.board.game_result

    def get_current_player(self) -> IPlayer:
        return self.white_player if self.board.active_player == Color('White') else self.black_player

    def set_players(self, white: str, black: str) -> None:
        self.white_player: IPlayer = Player(white) if white.lower() != 'human' else HumanPlayer(self.view.chessboard_view)
        self.black_player: IPlayer = Player(black) if black.lower() != 'human' else HumanPlayer(self.view.chessboard_view)

    def resign(self) -> Color:
        # do other stuff after resigning?
        self.playing = False
        return self.board.active_player

    def initialize_from_fen(self, fen_string: str) -> None:
        self.board.initialize_from_fen(fen_string)
    
    def do_move(self, move: Move):
        self.board.do_move(move)

    def undo_move(self):
        self.board.undo_move()