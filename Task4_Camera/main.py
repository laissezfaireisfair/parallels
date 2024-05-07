from logging import getLogger, basicConfig, INFO
from pathlib import Path
from queue import Queue
from threading import Thread, Event
from time import sleep
from cv2 import waitKey

from ProgramArguments import ProgramArguments
from SensorX import SensorX

logger = getLogger(__name__)


def sensor_x_job(stop_event, delay: float, update_period: float, result_queue: Queue):
    sensor = SensorX(delay)
    while not stop_event.wait(update_period):
        result_queue.put(sensor.get())


def get_last(queue: Queue, default: float) -> float:
    last = default
    while not queue.empty():
        last = queue.get()
    return last


class BoolWrapper:
    def __init__(self, value: bool):
        self.value = bool(value)


def set_exit_flag_on_key(flag: BoolWrapper):
    while waitKey(0) != ord('q'):
        pass
    flag.value = True


def main():
    sensor_request_period = 0.0001

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

    logger.info('Creating threads...')
    try:
        stop_event = Event()
        low_delay_results = Queue()
        low_delay_thread = Thread(target=sensor_x_job,
                                  args=(stop_event, 0.0001, sensor_request_period, low_delay_results))
        low_delay_thread.start()

        mid_delay_results = Queue()
        mid_delay_thread = Thread(target=sensor_x_job,
                                  args=(stop_event, 0.01, sensor_request_period, mid_delay_results))
        mid_delay_thread.start()

        high_delay_results = Queue()
        high_delay_thread = Thread(target=sensor_x_job,
                                   args=(stop_event, 1, sensor_request_period, high_delay_results))
        high_delay_thread.start()
    except Exception as exception:
        logger.error(exception)
        return

    is_close_requested = BoolWrapper(False)
    key_monitor_thread = Thread(target=set_exit_flag_on_key, args=(is_close_requested,))
    key_monitor_thread.start()

    last_low_delay_result = None
    last_mid_delay_result = None
    last_high_delay_result = None
    while not is_close_requested.value:
        last_low_delay_result = get_last(low_delay_results, last_low_delay_result)
        last_mid_delay_result = get_last(mid_delay_results, last_mid_delay_result)
        last_high_delay_result = get_last(high_delay_results, last_high_delay_result)
        print(f'{last_low_delay_result} {last_mid_delay_result} {last_high_delay_result}')
        sleep(1 / program_arguments.result_frequency)

    key_monitor_thread.join()

    logger.info('Invoking stop event...')
    try:
        stop_event.set()
    except Exception as exception:
        logger.error(exception)
        return

    logger.info('Waiting for thread to join...')
    low_delay_thread.join()
    mid_delay_thread.join()
    high_delay_thread.join()

    logger.info('Finish')


main()
