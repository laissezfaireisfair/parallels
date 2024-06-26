from logging import getLogger, basicConfig, INFO
from pathlib import Path
from queue import Queue
from threading import Thread, Event

import cv2

from ProgramArguments import ProgramArguments
from SensorCam import SensorCam
from SensorX import SensorX
from WindowImage import WindowImage

logger = getLogger(__name__)


def sensor_x_job(stop_event: Event, delay: float, update_period: float, result_queue: Queue):
    try:
        sensor = SensorX(delay)
        while not stop_event.wait(update_period):
            result_queue.put(sensor.get())
    except Exception as exception:
        logger.exception('SensorX job failed', exc_info=exception)


def sensor_cam_job(stop_event: Event, name: str, resolution: tuple[int, int], update_period: float,
                   result_queue: Queue):
    try:
        sensor = SensorCam(name, resolution)
        while not stop_event.wait(update_period):
            result_queue.put(sensor.get())
    except Exception as exception:
        logger.exception('SensorCam job failed', exc_info=exception)


def get_last(queue: Queue, default):
    last = default
    while not queue.empty():
        last = queue.get()
    return last


def main():
    sensor_request_period = .0001

    Path('logs/').mkdir(exist_ok=True)
    basicConfig(level=INFO, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
                datefmt='%d-%m-%y %H:%M:%S', filename='logs/info_plus.log', filemode='a')
    logger.info('Start up')

    logger.info('Parsing arguments...')
    try:
        program_arguments = ProgramArguments()
    except Exception as exception:
        logger.exception('', exc_info=exception)
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

        cam_results = Queue()
        cam_thread = Thread(target=sensor_cam_job,
                            args=(stop_event, program_arguments.camera_name, program_arguments.camera_resolution,
                                  sensor_request_period, cam_results))
        cam_thread.start()
    except Exception as exception:
        logger.exception('Creating threads failed', exc_info=exception)
        return

    logger.info('Creating image window...')
    try:
        window_image = WindowImage(program_arguments.result_frequency)
    except Exception as exception:
        logger.exception('Creating WindowImage failed', exc_info=exception)
        return

    last_low_delay_result = None
    last_mid_delay_result = None
    last_high_delay_result = None
    last_cam_image = None
    while not window_image.is_close_requested:
        last_low_delay_result = get_last(low_delay_results, last_low_delay_result)
        last_mid_delay_result = get_last(mid_delay_results, last_mid_delay_result)
        last_high_delay_result = get_last(high_delay_results, last_high_delay_result)
        last_cam_image = get_last(cam_results, last_cam_image)

        if last_cam_image is not None:
            try:
                text = f'{last_low_delay_result} - {last_mid_delay_result} - {last_high_delay_result}'
                cv2.putText(img=last_cam_image, text=text, org=(10, 30), fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=0.5,
                            color=(255, 255, 255), thickness=1, lineType=cv2.LINE_AA)
                window_image.show(last_cam_image)
            except Exception as exception:
                logger.exception('Frame show failed', exc_info=exception)

    logger.info('Invoking stop event...')
    try:
        stop_event.set()
    except Exception as exception:
        logger.exception('Stopping failed', exc_info=exception)
        return

    logger.info('Waiting for thread to join...')
    low_delay_thread.join()
    mid_delay_thread.join()
    high_delay_thread.join()

    logger.info('Finish')


main()
