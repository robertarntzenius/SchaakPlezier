import logging
from enum import Enum
from pathlib import Path
from typing import Optional

from pydantic import BaseModel as pydantic_BaseModel
from pydantic import Field, field_validator
from wrappers import Board, PlayerType


class BaseModel(pydantic_BaseModel):
    class Config:
        arbitrary_types_allowed = True  # required to allow PlayerType as a type here


class RGBAColor(BaseModel):
    values: list[int | float] = Field(min_length=4, max_length=4)

    @field_validator("values")
    def check_values(cls, v):
        if any(component > 255 for component in v):
            raise ValueError("Each component of the RGBA color must be less than or equal to 255.")
        return v

    def to_hex(self):
        rgba = [
            (int(value * 255) if isinstance(value, float) else int(value)) for value in self.values
        ]
        rgba = [max(0, min(255, value)) for value in rgba]
        hex_color = "#{:02X}{:02X}{:02X}{:02X}".format(rgba[0], rgba[1], rgba[2], rgba[3])
        return HexColor(value=hex_color)


class HexColor(BaseModel):
    value: str = Field(pattern=r"^#(?:[0-9a-fA-F]{3}|[0-9a-fA-F]{6})$")

    def to_rgba(self):
        hex_color = self.value.lstrip("#")
        r = int(hex_color[0:2], 16)
        g = int(hex_color[2:4], 16)
        b = int(hex_color[4:6], 16)
        a = (
            int(hex_color[6:8], 16) / 255 if len(hex_color) == 8 else 1.0
        )  # Default alpha to 1.0 if not provided

        return RGBAColor(values=[r, g, b, a])


class FENString(BaseModel):
    string: str = Field(
        default="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", validate_default=True
    )

    @field_validator("string", mode="before")
    @classmethod
    def valid_position(cls, value):
        try:
            _ = Board(value, "")
            return value
        except Exception as e:
            raise ValueError(f"Invalid FENstring: {value}. Validation failed: {e}")


DARK_BROWN = HexColor(value="#946f51")
LIGHT_BROWN = HexColor(value="#f0d9b5")
YELLOW = RGBAColor(values=[255, 255, 0, 100])
PINK = RGBAColor(values=[255, 200, 200, 100])
RED = RGBAColor(values=[255, 0, 0, 100])
BLUE = RGBAColor(values=[0, 0, 255, 100])


class SquareColors(BaseModel):
    fill: RGBAColor
    border: RGBAColor


class ColorConfig(BaseModel):
    light_color: HexColor = Field(default=LIGHT_BROWN)
    dark_color: HexColor = Field(default=DARK_BROWN)

    previous_move: SquareColors = Field(
        default=SquareColors(
            fill=YELLOW,
            border=YELLOW,
        )
    )
    selected_square: SquareColors = Field(
        default=SquareColors(
            fill=PINK,
            border=RED,
        )
    )
    possible_moves: SquareColors = Field(
        default=SquareColors(
            fill=PINK,
            border=BLUE,
        )
    )


class GUIConfig(BaseModel):
    title: str = Field(default="SchaakPlezier")
    colors: ColorConfig = Field(default=ColorConfig())


class LogConfig(BaseModel):
    log_file: Optional[Path] = Field(default=None)
    log_level_dialog: int = Field(
        default=logging.INFO, multiple_of=10, ge=logging.NOTSET, le=logging.CRITICAL
    )
    log_level_files: int = Field(
        default=logging.DEBUG, multiple_of=10, ge=logging.NOTSET, le=logging.CRITICAL
    )
    log_level_console: int = Field(
        default=logging.INFO, multiple_of=10, ge=logging.NOTSET, le=logging.CRITICAL
    )


class BoardConfig(BaseModel):
    fen_string: FENString = Field(default=FENString())
    white_player: PlayerType = Field(default=PlayerType.Human)
    black_player: PlayerType = Field(default=PlayerType.AlphaBeta)

    log_file: Optional[Path] = Field(default=None)
    log_level: int = Field(
        default=logging.INFO, multiple_of=10, ge=logging.NOTSET, le=logging.CRITICAL
    )


class Settings(BaseModel):
    gui: GUIConfig = Field(default=GUIConfig())
    log: LogConfig = Field(default=LogConfig())
    board: BoardConfig = Field(default=BoardConfig())


class Mode(str, Enum):
    IDLE = "IDLE"
    PLAYING = "PLAYING"
    EDIT = "EDIT"
