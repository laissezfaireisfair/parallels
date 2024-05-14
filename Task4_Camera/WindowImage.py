from logging import getLogger

import cv2

logger = getLogger(__name__)


class WindowImage:
    def __init__(self, frequency: float):
        self.delay = round(1000 / frequency)
        self.window_name = 'Image window'
        self.is_close_requested = False

    def show(self, image):
        cv2.imshow(self.window_name, image)
        if cv2.waitKey(self.delay) == ord('q'):
            self.is_close_requested = True

    def __del__(self):
        cv2.destroyWindow(self.window_name)
