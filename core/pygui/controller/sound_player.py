from PyQt5.QtMultimedia import QSoundEffect
from PyQt5.QtCore import QUrl

from core.pygui.view.observer import ObserverWidget
from enum import Enum
from core.pygui import ASSETS_DIR


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
            Sound.game_start: str(ASSETS_DIR / "sounds" / "game-start.wav").replace('\\\\', '/'),
            Sound.game_end: str(ASSETS_DIR / "sounds" / "game-end.wav").replace('\\\\', '/'),
            Sound.time_remaining: str(ASSETS_DIR / "sounds" / "tenseconds.wav").replace('\\\\', '/'),
            Sound.normal_white: str(ASSETS_DIR / "sounds" / "move-self.wav").replace('\\\\', '/'),
            Sound.normal_black: str(ASSETS_DIR / "sounds" / "move-opponent.wav").replace('\\\\', '/'),
            Sound.check: str(ASSETS_DIR / "sounds" / "move-check.wav").replace('\\\\', '/'),
            Sound.castle: str(ASSETS_DIR / "sounds" / "castle.wav").replace('\\\\', '/'),
            Sound.capture: str(ASSETS_DIR / "sounds" / "capture.wav").replace('\\\\', '/'),
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

    def play(self, sound: Sound):
        if sound not in self._sounds:
            raise ValueError(f'Invalid sound request: {sound}')
        self._sounds[sound].play()