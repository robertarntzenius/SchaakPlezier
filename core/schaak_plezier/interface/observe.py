from abc import ABC, ABCMeta, abstractmethod

from PyQt5.QtWidgets import QWidget


class QABCMeta(ABCMeta, type(QWidget)):
    """Create a meta class that combines ABC and the Qt meta class"""

    pass


class ABC_Widget(ABC, metaclass=QABCMeta):
    """Abstract class, to be multi-inherited together with a Qt item"""

    def __init__(self, parent=None):
        QWidget.__init__(self, parent)


class IObserver(ABC_Widget):
    def notify(): ...


class IObservable(ABC):
    def register_observer(self, observer: IObserver):
        self._observers.append(observer)

    def unregister_observer(self, observer: IObserver):
        self._observers.remove(observer)

    def notify_observers(self, **kwargs):
        for obs in self._observers:
            obs.notify(**kwargs)


class ObservableWidget(IObservable):
    _observers: list[IObserver]

    def __init__(self, parent=None):
        self._observers: list[IObserver] = []


class ObserverWidget(IObserver, QWidget):
    def __init__(self, observable_list: list[IObservable], parent=None):
        QWidget.__init__(self, parent=parent)

        for observable in observable_list:
            observable.register_observer(self)

    @abstractmethod
    def notify(self, **kwargs): ...
