from abc import abstractmethod
from typing import Optional

from PyQt5.QtWidgets import QWidget


class Observer:
    def __init__(self, observable: "Observable") -> None:
        observable.register_observer(self)

    @abstractmethod
    def notify(self, **kwargs): ...


class Observable:
    _observers: list[Observer]

    def __init__(self):
        self._observers: list[Observer] = []

    def register_observer(self, observer: Observer):
        self._observers.append(observer)

    def unregister_observer(self, observer: Observer):
        self._observers.remove(observer)

    def notify_observers(self, **kwargs):
        for obs in self._observers:
            obs.notify(**kwargs)


class ObserverWidget(Observer, QWidget):
    def __init__(self, observable: Observable, parent: Optional[QWidget]):
        QWidget.__init__(self, parent=parent)
        Observer.__init__(self, observable)
