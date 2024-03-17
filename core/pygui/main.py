import sys
from PyQt5.QtWidgets import QApplication
from app import SchaakPlezierApp

def main():
    app = QApplication(sys.argv)
    window = SchaakPlezierApp()
    window.show()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()