import logging
from pathlib import Path

from pydantic import BaseModel, Field, field_validator


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


class SquareColors(BaseModel):
    fill: RGBAColor
    border: RGBAColor


class ColorConfig(BaseModel):
    light_color: HexColor
    dark_color: HexColor

    previous_move: SquareColors
    selected_square: SquareColors
    possible_moves: SquareColors


class GUIConfig(BaseModel):
    title: str
    fen_string: str
    log_file: Path
    log_level: int = Field(multiple_of=10, ge=logging.NOTSET, le=logging.CRITICAL)
    white_player: str
    black_player: str
    colors: ColorConfig
