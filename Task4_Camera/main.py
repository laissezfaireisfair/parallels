from pathlib import Path
from logging import getLogger, basicConfig, INFO
from time import sleep
from threading import Thread

from ProgramArguments import ProgramArguments
from SensorCam import SensorCam
from SensorX import SensorX

logger = getLogger(__name__)


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
