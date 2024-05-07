from pathlib import Path
from logging import getLogger, basicConfig, INFO
from time import sleep
from threading import Thread
from argparse import ArgumentParser

logger = getLogger(__name__)


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
    def __init__(self, name: str, resolution: str):
        super().__init__(is_valid_child=True)
        self.name = name
        self.resolution = resolution

    def get(self):
        raise NotImplementedError('Not yet implemented')

    def __del__(self):
        pass


class WindowImage:
    def __init__(self, frequency: float):
        self.frequency = frequency

    def show(self, image):
        raise NotImplementedError('Not yet implemented')

    def __del__(self):
        pass


class ProgramArguments:
    def __init__(self):
        parser = ArgumentParser(description='Sensor')
        parser.add_argument('cameraName', type=str, help='System name of the camera')
        parser.add_argument('cameraResolution', type=str, help='Camera resolution')
        parser.add_argument('resultFrequency', type=str, help='Result update frequency')

        args = parser.parse_args()
        self.camera_name = args.cameraName
        self.camera_resolution = args.cameraResolution
        self.camera_frequency = float(args.cameraFrequency)

    def __str__(self):
        return f'{self.camera_name} {self.camera_resolution} {self.camera_frequency}'


def job(program_arguments: ProgramArguments):
    logger.info(f'Job started with program arguments: {program_arguments}')

    sleep(0.5)

    cam_sensor = SensorCam(program_arguments.camera_name, program_arguments.camera_resolution)

    fast_sensor = SensorX(0.01)
    mid_sensor = SensorX(0.1)
    slow_sensor = SensorX(1)

    logger.info('Job finished')


def main():
    Path('logs/').mkdir(exist_ok=True)
    basicConfig(level=INFO, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
                datefmt='%d-%m-%y %H:%M:%S', filename='logs/info_plus.log', filemode='a')
    logger.info('Start up')

    logger.info('Parsing arguments...')

    try:
        program_arguments = ProgramArguments()
    except Exception as exception:
        logger.error(exception)
        return

    logger.info('Starting thread...')

    thread = Thread(target=job, args=(program_arguments,))
    thread.start()

    logger.info('Waiting for thread to finish...')

    thread.join()

    logger.info('Finish')


main()
