from enum import Enum

from PyQt5.QtCore import QUrl
from PyQt5.QtMultimedia import QSoundEffect

from pygui import ASSETS_DIR
from pygui.interface.observe import Observable, Observer


class Sound(str, Enum):
    game_start = "game_start"
    game_end = "game_end"
    time_remaining = "time_remaining"
    castle = "castle"
    capture = "capture"
    check = "check"
    normal_white = "normal_white"
    normal_black = "normal_black"


class SoundPlayer(Observer):
    def __init__(self, observable: Observable):
        super().__init__(observable=observable)
        self._sound_files: dict[Sound, str] = {
            Sound.game_start: (ASSETS_DIR / "sounds" / "game-start.wav").as_posix(),
            Sound.game_end: (ASSETS_DIR / "sounds" / "game-end.wav").as_posix(),
            Sound.time_remaining: (ASSETS_DIR / "sounds" / "tenseconds.wav").as_posix(),
            Sound.normal_white: (ASSETS_DIR / "sounds" / "move-self.wav").as_posix(),
            Sound.normal_black: (ASSETS_DIR / "sounds" / "move-opponent.wav").as_posix(),
            Sound.check: (ASSETS_DIR / "sounds" / "move-check.wav").as_posix(),
            Sound.castle: (ASSETS_DIR / "sounds" / "castle.wav").as_posix(),
            Sound.capture: (ASSETS_DIR / "sounds" / "capture.wav").as_posix(),
        }
        self._sounds: dict[Sound, QSoundEffect] = {
            sound: self._create_sound_effect(file) for sound, file in self._sound_files.items()
        }

    def _create_sound_effect(self, file_path):
        sound_effect = QSoundEffect()
        sound_effect.setSource(QUrl.fromLocalFile(file_path))
        sound_effect.setVolume(1)

        return sound_effect

    def notify(self, sound=None, **kwargs):
        if sound is not None:
            self.play(sound)

    def play(self, sound: Sound):
        if sound not in self._sounds:
            raise ValueError(f"Invalid sound request: {sound}")
        self._sounds[sound].play()
