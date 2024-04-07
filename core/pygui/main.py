import sys
from PyQt5.QtWidgets import QApplication
from controller.controller import Controller
from controller.config import load_config

def main():
    app = QApplication(sys.argv)    
    
    config = load_config()
    controller = Controller(config)
    
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()