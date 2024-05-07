from Sensor import Sensor
from logging import getLogger

logger = getLogger(__name__)


class SensorCam(Sensor):
    def __init__(self, name: str, resolution: str):
        super().__init__(is_valid_child=True)
        self.name = name
        self.resolution = resolution

    def get(self):
        raise NotImplementedError('Not yet implemented')

    def __del__(self):
        pass
