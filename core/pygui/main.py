from pathlib import Path
import tkinter as tk
from tkinter import messagebox
from PIL import Image, ImageTk  # Pillow library for image processing (install using pip install Pillow)
import wrappers

class Font():
    type = "Helvetica"
    color = "#000000"
    size = 25


class Theme():
    light_color = "#ccffff"
    dark_color =  "#873e3e"
    font = Font()
    

class GUI_config():
    window_width = 600
    window_height = 600
    board_height = 400
    board_width = 400
    title = "SchaakPlezier"
    fen_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    log_file = "GUI.log"
    fps = 60
    refresh_time = int((1 / fps) * 1000)
    theme = Theme()
    board_dimensions = 8
    

class ChessGUI:
    def __init__(self, master, config: GUI_config):
        self.config = config
        self.master = master
        self.master.title(config.title)
        self.board = wrappers.Board(config.fen_string, config.log_file)
        self.create_widgets()

    def create_widgets(self):
        self.canvas = tk.Canvas(self.master, width=self.config.window_width, height=self.config.window_height)
        self.canvas.pack()
        self.update_board()

    def update_board(self):
        self.draw_board()
        self.master.after((self.config.refresh_time), self.update_board)

    def draw_board(self):
        v_padding = (self.config.window_height - self.config.board_height) // 2
        h_padding = (self.config.window_width - self.config.board_width) // 2

        colors = [self.config.theme.light_color, self.config.theme.dark_color]  # Light and dark square colors
        squaresize = self.config.board_height // 8

        # Background
        for row in range(self.config.board_dimensions):
            for col in range(self.config.board_dimensions):
                color = colors[(row + col) % 2]
                x0, y0 = (col * squaresize) + h_padding, (row * squaresize) + v_padding
                x1, y1 = (x0 + squaresize), (y0 + squaresize)
            
                background = self.canvas.create_rectangle(x0, y0, x1, y1, fill=color)
                self.canvas.tag_raise(background)

        # Pieces
        top_left = h_padding, v_padding
        bottom_right = self.config.board_width + h_padding, self.config.board_height + v_padding

        wpieces, bpieces = self.board.getPieceMaps()
        for piece in wpieces:
            square = piece[0]
            ptype = piece[1]
            x, y = self.square_to_pixel(square, squaresize, h_padding, v_padding)
            self.draw_piecechar(ptype, "white", x, y)
        
        for piece in bpieces:
            square = piece[0]
            ptype = piece[1]
            x, y = self.square_to_pixel(square, squaresize, h_padding, v_padding)
            self.draw_piecechar(ptype, "black", x, y)

    def square_to_pixel(self, square, squaresize, h_padding, v_padding):
        col = ord(square.name[0]) - ord('a')
        row = 8 - int(square.name[1])

        x = col * squaresize + h_padding + (squaresize / 2)
        y = row * squaresize + v_padding + (squaresize / 2)
        return x, y
    
    def draw_piece(self, ptype, color, x, y):
        # draw the piece from the assets folder containg .pngs 
        # with w or b as first character and piece char as last character
        color = color[0].lower()
        ptype = str(ptype).split('.')[1]
        if ptype == "Knight":
            ptype = "n"
        else:
            ptype = ptype[0].lower()
        squaresize = self.config.board_height // 8
        image_path = Path(f"assets/{color}{ptype}.png")
        assert image_path.exists()

        image = Image.open(image_path)
        image = image.resize((squaresize, squaresize))
        image = ImageTk.PhotoImage(image)

        self.canvas.create_image(x, y, image=image)

    def draw_piecechar(self, ptype, color, x, y):
        piece_str = str(ptype).split('.')[1]

        if piece_str == "Knight":
            piece_char = "N"
        else:
            piece_char = piece_str[0]

        piece_char = piece_char.upper() if color == "white" else piece_char.lower()
        font = f"{self.config.theme.font.type} {self.config.theme.font.size}"
        
        self.canvas.create_text(x, y, text=piece_char, font=font, fill=self.config.theme.font.color)


def main():
    config = GUI_config()
    config.fen_string = "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/8/PPPBBPPP/R3K2R b KQ - 3 2"

    root = tk.Tk()
    chess_gui = ChessGUI(root, config)
    root.mainloop()

if __name__ == "__main__":
    main()
