from PyQt5.QtMultimedia import QSoundEffect
from PyQt5.QtCore import QUrl

from core.pygui.view.observer import ObserverWidget
from enum import Enum

class Sound(str, Enum):
    game_start = "game_start"
    game_end = "game_end"
    time_remaining = "time_remaining"
    castle = "castle"
    capture = "capture"
    check = "check"
    normal_white = "normal_white"
    normal_black = "normal_black"


class SoundPlayer(ObserverWidget):
    def __init__(self, observable_list):
        super().__init__(observable_list=observable_list)
        self._sound_files: dict[Sound, str] = {
            Sound.game_start: "./assets/sounds/game-start.wav",
            Sound.game_end: "./assets/sounds/game-end.wav",
            Sound.time_remaining: "./assets/sounds/tenseconds.wav",
            Sound.normal_white: "./assets/sounds/move-self.wav",
            Sound.normal_black: "./assets/sounds/move-opponent.wav",
            Sound.check: "./assets/sounds/move-check.wav",
            Sound.castle: "./assets/sounds/castle.wav",
            Sound.capture: "./assets/sounds/capture.wav",
        }
        self._sounds: dict[Sound, QSoundEffect] = {sound: self._create_sound_effect(file) for sound, file in self._sound_files.items()}

    def _create_sound_effect(self, file_path):
        sound_effect = QSoundEffect()
        sound_effect.setSource(QUrl.fromLocalFile(file_path))
        sound_effect.setVolume(1)
        return sound_effect

    def notify(self, sound=None):
        if sound is not None:
            self.play(sound)

    def play(self, sound: Sound = None):
        if sound not in self._sounds:
            raise ValueError(f'Invalid sound request: {sound}')
        self._sounds[sound].play()