from abc import ABC
from core.pygui.view.observer import ObserverWidget

class Observable(ABC):
    def __init__(self, ):
        self._observers: list[ObserverWidget] = []

    def register_observer(self, observer: ObserverWidget):
        self._observers.append(observer)

    def notify_observers(self, **kwargs):
        for obs in self._observers:
            obs.notify(**kwargs)
