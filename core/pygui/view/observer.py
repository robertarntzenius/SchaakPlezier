from abc import ABC, ABCMeta, abstractmethod
from PyQt5.QtWidgets import QWidget


class QABCMeta(ABCMeta, type(QWidget)):
    """Create a meta class that combines ABC and the Qt meta class"""
    pass


class TcWidget(ABC, metaclass=QABCMeta):
    """Abstract class, to be multi-inherited together with a Qt item"""
    pass


class ObserverWidget(QWidget, TcWidget):
    def __init__(self, observable_list: list, parent=None):
        super().__init__(parent=parent)
        self.parent = parent
        
        if not isinstance(observable_list, list):
            observable_list = [observable_list]

        for observable in observable_list:
            observable.register_observer(self)

    @abstractmethod
    def notify(self, **kwargs):
        ...