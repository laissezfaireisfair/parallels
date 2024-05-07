class WindowImage:
    def __init__(self, frequency: float):
        self.frequency = frequency

    def show(self, image):
        raise NotImplementedError('Not yet implemented')

    def __del__(self):
        pass
