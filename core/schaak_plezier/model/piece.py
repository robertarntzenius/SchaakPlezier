from PyQt5.QtCore import QSize, Qt
from PyQt5.QtGui import QImage, QPainter

from schaak_plezier import ASSETS_DIR
from schaak_plezier.interface.wrapper_types import Color, Piecetype, Square


class Piece:
    square: Square
    piece_type: Piecetype
    color: Color
    image: QImage

    def __init__(self, square: Square, piece_type: Piecetype, color: Color):
        self.square = square
        self.piece_type = piece_type
        self.color = color
        self.image = self.load_image()

    def load_image(self):
        color_char = "w" if self.color == Color("White") else "b"
        piece_type_char = self.piece_type.name[0].lower()
        if self.piece_type == Piecetype("Knight"):
            piece_type_char = "n"

        filename = (ASSETS_DIR / "images" / f"{color_char}{piece_type_char}.png").as_posix()
        image = QImage(filename)
        return image

    def draw(self, qp: QPainter, board_size: int):
        cell_size = board_size // 8
        row = self.square.value // 8
        col = self.square.value % 8
        x_pos = col * cell_size
        y_pos = row * cell_size

        square_size = QSize(cell_size, cell_size)
        scaled_image = self.image.scaled(square_size, aspectRatioMode=Qt.KeepAspectRatio)
        qp.drawImage(x_pos, y_pos, scaled_image)
