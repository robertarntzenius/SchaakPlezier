from abc import ABC

class Observable(ABC):
    def __init__(self, ):
        self._observers = []

    def register_observer(self, observer):
        self._observers.append(observer)

    def notify_observers(self):
        for obs in self._observers:
            obs.notify()
