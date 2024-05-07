from time import sleep

from Sensor import Sensor
from logging import getLogger

logger = getLogger(__name__)


class SensorX(Sensor):
    def __init__(self, delay: float):
        super().__init__(is_valid_child=True)
        self.delay = delay
        self.data = 0

    def get(self) -> int:
        sleep(self.delay)
        self.data += 1
        return self.data
