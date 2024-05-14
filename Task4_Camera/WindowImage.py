from logging import getLogger

logger = getLogger(__name__)


class WindowImage:
    def __init__(self, frequency: float):
        self.frequency = frequency  # TODO: Consider getting rid of

    def show(self, image):
        raise NotImplementedError('Not yet implemented')

    def __del__(self):
        pass
