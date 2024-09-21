import logging
import sys
from contextlib import contextmanager
from pathlib import Path
from typing import Optional

from schaak_plezier.config import SETTINGS


class FixedWidthFormatter(logging.Formatter):
    def format(self, record):
        log_level = f"{record.levelname:<5}"
        logger_name = f"{record.name:<40}"
        message = record.getMessage()
        return f"{log_level} | {logger_name} | {message}"


class SchaakPlezierLogging:
    _DEFAULT_FORMATTER = FixedWidthFormatter()

    _root_logger = logging.getLogger()

    def __init__(
        self,
        file_path: Optional[Path] = SETTINGS.log_file,
        loglevel_console: int = SETTINGS.log_level,
        loglevel_root: int = SETTINGS.log_level,
        loglevel_files: int = SETTINGS.log_level,
        formatter: logging.Formatter = _DEFAULT_FORMATTER,
    ) -> None:
        """Initialize the logging system for the SchaakPlezier."""
        self._formatter = formatter

        self._root_logger.setLevel(loglevel_root)

        # Add file handler if provided
        if file_path is not None:
            self.add_file_handler(file_path, loglevel_files, formatter)

        # Add console handler
        console_handler = logging.StreamHandler(stream=sys.stdout)
        console_handler.setLevel(loglevel_console)
        console_handler.setFormatter(formatter)
        self.getLogger().addHandler(console_handler)

    @classmethod
    def add_file_handler(
        cls,
        file_path: Path,
        loglevel: int = logging.DEBUG,
        formatter: logging.Formatter = _DEFAULT_FORMATTER,
    ) -> None:
        """Add a file handler to the logger that directs outputs to a the file."""
        path = Path(file_path).resolve()
        if not path.parent.exists():
            path.parent.mkdir()

        file_handler = logging.FileHandler(filename=path, mode="w+")
        file_handler.setLevel(loglevel)
        file_handler.setFormatter(formatter)

        cls.getLogger().addHandler(file_handler)
        cls.getLogger().debug(f"Added file handler. Writing to {path}")

    @classmethod
    def remove_file_handler(cls, file_path: Path) -> None:
        """Remove a file handler from the logger, which stops sending logs to that file and closes it."""
        for handler in cls.getLogger().handlers:
            if isinstance(handler, logging.FileHandler) and handler.baseFilename == str(
                file_path.resolve()
            ):
                handler.close()
                cls.getLogger().removeHandler(handler)

    @classmethod
    def getLogger(cls, name: Optional[str] = None, level: Optional[int] = None) -> logging.Logger:
        """Get a logger with the specified name. If no name is provided, return the root logger.

        If the logger does not exist, it is created with the specified level. If no level is provided, the logger inherits the level of the root logger.

        Parameters
        ----------
        name : str, optional
            The name of the logger. If not provided, the root logger is returned.
        level : int, optional
            The level of the logger. If not provided, the logger inherits the level of the root logger.

        Returns
        -------
        logging.Logger
            The logger with the specified name.

        """
        if name is None:
            logger = cls._root_logger
        else:
            logger = logging.getLogger(name)

        if level is not None:
            logger.setLevel(level)

        return logger

    @classmethod
    def shutdown(cls):
        root_logger = cls.getLogger()
        handlers = root_logger.handlers[:]
        for handler in handlers:
            if isinstance(handler, logging.FileHandler):
                handler.close()
                root_logger.removeHandler(handler)
        logging.shutdown()

    @classmethod
    @contextmanager
    def to_file(
        cls,
        *,
        file_path: Path,
        loglevel: int = logging.DEBUG,
        formatter: logging.Formatter = _DEFAULT_FORMATTER,
    ):
        """Open a file at filepath to write logs to. Does not affect other loggers.

        When the context manager exits (via regular execution or an exception), the file is closed and the handler is removed.
        """
        if file_path is None:
            raise ValueError(
                "file_path must be provided as a key value pair: 'file_path=<file_path>'."
            )
        cls.add_file_handler(file_path, loglevel, formatter)
        try:
            yield
        finally:
            cls.remove_file_handler(file_path)
