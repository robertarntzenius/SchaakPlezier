from PyQt5.QtMultimedia import QSound

from core.pygui.view.observer import ObserverWidget
from enum import Enum, auto

class Sound(Enum):
    game_start = auto()
    game_end = auto()
    time_remaining = auto()

    castle = auto()
    capture = auto()
    check = auto()
    normal_white = auto()
    normal_black = auto()


class SoundPlayer():
    def __init__(self):
        super().__init__()
        self._sound_files = {
            Sound.game_start: "./assets/sounds/game-start.wav",
            Sound.game_end: "./assets/sounds/game-end.wav",
            Sound.time_remaining: "./assets/sounds/tenseconds.wav",
            Sound.normal_white: "./assets/sounds/move-self.wav",
            Sound.normal_black: "./assets/sounds/move-opponent.wav",
            Sound.check: "./assets/sounds/move-check.wav",
            Sound.castle: "./assets/sounds/castle.wav",
            Sound.capture: "./assets/sounds/capture.wav",
        }
        self.sounds = {sound: QSound(file) for sound, file in self._sound_files.items()}

    def play(self, sound: Sound = None):
        if sound is None:
            return
        
        if sound not in self.sounds:
            raise ValueError(f'Invalid sound request: {sound}')
        self.sounds[sound].play()
