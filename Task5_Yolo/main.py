from argparse import ArgumentParser

import cv2
from ultralytics import YOLO
from time import time, sleep
from queue import Queue
from threading import Thread, Event


def worker(stop_event: Event, queue: Queue, results: dict):
    model = YOLO('yolov8s-pose.pt')

    while not stop_event.is_set():
        if queue.empty():
            sleep(0.01)
            continue

        (frame, index) = queue.get()
        annotated_frame = model.predict(frame, device='cpu')[0].plot()
        results[index] = annotated_frame


def main():
    parser = ArgumentParser(description='Yolo')
    parser.add_argument('vidPath', type=str, help='Path to video')
    parser.add_argument('mode', type=str, help='Single-threaded or multi-threaded')
    parser.add_argument('outputName', type=str, help='Name of output file')

    args = parser.parse_args()
    video_path = args.vidPath
    mode = args.mode
    if mode != 'singlethread' and mode != 'multithread':
        print('Unknown mode. Expected singlethread or multithread Exiting...')
        return
    output_name = args.outputName

    video = cv2.VideoCapture(video_path)
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    writer = cv2.VideoWriter(output_name, fourcc, 20.0, (640, 480))

    processing_begin_time = time()

    # model = YOLO('yolov8s-pose.pt')
    #
    # while video.isOpened():
    #     ret, frame = video.read()
    #     if not ret:
    #         print('Cannot read frame. Exiting...')
    #         break
    #     annotated_frame = model.predict(frame, device='cpu')[0].plot()
    #     writer.write(annotated_frame)

    task_queue = Queue()
    result_by_index = {}

    stop_event = Event()
    threads_count = 8
    threads = [Thread(target=worker, args=(stop_event, task_queue, result_by_index)) for _ in range(threads_count)]
    for thread in threads:
        thread.start()

    counter = 0
    while video.isOpened():
        ret, frame = video.read()
        if not ret:
            break
        task_queue.put((frame, counter))
        counter += 1

    while not task_queue.empty():
        sleep(0.01)
    stop_event.set()

    counter = 0
    while result_by_index[counter] is not None:
        writer.write(result_by_index[counter])
        counter += 1

    process_time = time() - processing_begin_time

    writer.release()
    video.release()
    cv2.destroyAllWindows()

    print(f'Processing took {process_time:.2f} seconds')


main()
