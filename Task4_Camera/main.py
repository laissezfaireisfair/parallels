from pathlib import Path
import logging
from time import sleep
import threading
import queue
logger = logging.getLogger(__name__)


class Sensor:
    def __init__(self, is_valid_child: bool = False):
        if not is_valid_child:
            raise RuntimeError('Cannot instantiate abstract class Sensor')


class SensorX(Sensor):
    def __init__(self, delay: float):
        super().__init__(is_valid_child=True)
        self.delay = delay
        self.data = 0

    def get(self) -> int:
        sleep(self.delay)
        self.data += 1
        return self.data


class SensorCam(Sensor):
    def __init__(self):
        super().__init__(is_valid_child=True)
        raise NotImplementedError('Not yet implemented')

    def get(self):
        raise NotImplementedError('Not yet implemented')

    def __del__(self):
        raise NotImplementedError('Not yet implemented')


def job():
    logger.info("Job started")

    sleep(0.5)

    cam_sensor = SensorCam()

    fast_sensor = SensorX(0.01)
    mid_sensor = SensorX(0.1)
    slow_sensor = SensorX(1)

    logger.info("Job finished")


def main():
    Path("logs/").mkdir(exist_ok=True)
    logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s',
                        datefmt='%d-%m-%y %H:%M:%S', filename='logs/info_plus.log', filemode='a')
    logger.info("Start up")

    thread = threading.Thread(target=job)
    thread.start()

    logger.info("Now waiting for thread to finish")

    thread.join()
    logging.info("Finish")


main()
