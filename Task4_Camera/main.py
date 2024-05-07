import threading
from logging import getLogger, basicConfig, INFO
from pathlib import Path
from queue import Queue
from threading import Thread
from time import sleep

from ProgramArguments import ProgramArguments
from SensorX import SensorX

logger = getLogger(__name__)


def sensor_x_job(stop_event, delay: int, result_queue: Queue):
    sensor = SensorX(delay)
    while not stop_event.wait(1):
        result_queue.put(sensor.get())
        sleep(delay)


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
    logger.info(f'Program arguments: {program_arguments}')

    logger.info('Creating thread...')
    try:
        stop_event = threading.Event()
        delay = 1
        result_queue = Queue()
        sensor_x_thread = Thread(target=sensor_x_job, args=(stop_event, delay, result_queue))
    except Exception as exception:
        logger.error(exception)
        return

    logger.info('Starting thread...')
    try:
        sensor_x_thread.start()
    except Exception as exception:
        logger.error(exception)
        return

    logger.info('Waiting 10 sec...')
    sleep(10)

    logger.info('Invoking stop event...')
    try:
        stop_event.set()
    except Exception as exception:
        logger.error(exception)
        return

    logger.info('Waiting for thread to join...')
    sensor_x_thread.join()

    logger.info('Finish')


main()
