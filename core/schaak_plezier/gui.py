from typing import List

import tomli

from schaak_plezier import ASSETS_DIR
from schaak_plezier.controller.controller import Controller
from schaak_plezier.interface.app import ISchaakPlezier
from schaak_plezier.interface.config import GUIConfig
from schaak_plezier.interface.log import SchaakPlezierLogging
from schaak_plezier.view.view import View


class SchaakPlezier(ISchaakPlezier):
    def __init__(self, argv: List[str]) -> None:
        super().__init__(argv)

        with open(ASSETS_DIR / "config" / "config.toml", "rb") as toml:
            data = tomli.load(toml)

        self.config = GUIConfig.model_validate(data)
        self.logger = SchaakPlezierLogging(
            file_path=self.config.log_file, loglevel_root=self.config.log_level
        ).getLogger()

    def build(self):
        self.controller = Controller(self.config)
        self.view = View(self.controller, self.config)

        self.controller.connect_view(self.view)

    def run(self):
        if self.exec_() != 0:
            raise RuntimeError("Application exited with a non-zero exit code.")
