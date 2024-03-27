import sys
from PyQt5.QtWidgets import QApplication
from core.pygui.view.view import View
from model import Chessboard
from controller import Controller

def main():
    app = QApplication(sys.argv)
    
    board = Chessboard()
    
    view = View(board)

    controller = Controller(board, view)

    sys.exit(app.exec_())


if __name__ == '__main__':
    main()