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
    parser.add_argument('threadsCount', type=int, help='Count of worker threads')
    parser.add_argument('outputName', type=str, help='Name of output file')

    args = parser.parse_args()
    video_path = args.vidPath
    threads_count = args.threadsCount
    if threads_count < 1:
        print('Expected at least one worker thread')
        return
    output_name = args.outputName

    video = cv2.VideoCapture(video_path)
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    writer = cv2.VideoWriter(output_name, fourcc, 20.0, (640, 480))

    processing_begin_time = time()

    task_queue = Queue()
    result_by_index = {}

    stop_event = Event()
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
    while counter in result_by_index:
        writer.write(result_by_index[counter])
        counter += 1

    process_time = time() - processing_begin_time

    for thread in threads:
        thread.join()

    writer.release()
    video.release()
    cv2.destroyAllWindows()

    print(f'Processing took {process_time:.2f} seconds')


main()
