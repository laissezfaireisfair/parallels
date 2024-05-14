from logging import getLogger
import cv2

from Sensor import Sensor

logger = getLogger(__name__)


class SensorCam(Sensor):
    def __init__(self, name: str, resolution: str):
        super().__init__(is_valid_child=True)
        self.resolution = resolution
        self.camera = cv2.VideoCapture(name)

    def get(self):
        _, image = self.camera.read()
        return image

    def __del__(self):
        self.camera.release()
