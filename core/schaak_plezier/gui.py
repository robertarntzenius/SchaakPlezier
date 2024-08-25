from typing import List

import tomli

from schaak_plezier import ASSETS_DIR
from schaak_plezier.controller.controller import Controller
from schaak_plezier.interface.app import IController, ISchaakPlezier, IView
from schaak_plezier.interface.config import GUIConfig
from schaak_plezier.interface.log import FixedWidthFormatter, SchaakPlezierLogging
from schaak_plezier.view.view import View


class SchaakPlezier(ISchaakPlezier):
    controller: IController
    view: IView

    def __init__(self, argv: List[str]) -> None:
        super().__init__(argv)

        with open(ASSETS_DIR / "config" / "config.toml", "rb") as toml:
            data = tomli.load(toml)

        self.config = GUIConfig.model_validate(data)

        SchaakPlezierLogging(
            file_path=self.config.log_file,
            loglevel_root=self.config.log_level,
            formatter=FixedWidthFormatter(),
        )
        self.logger = SchaakPlezierLogging.getLogger(__name__)

    def build(self):
        self.controller = Controller(self.config)
        self.view = View(self.controller, self.config)

        self.controller.connect_view(self.view)

    def run(self):
        if self.exec_() != 0:
            raise RuntimeError("Application exited with a non-zero exit code.")
