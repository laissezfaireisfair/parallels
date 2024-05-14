from argparse import ArgumentParser
from queue import Queue
from threading import Event, Thread
from time import time, sleep

import cv2
from ultralytics import YOLO


class Application:
    def __init__(self):
        parser = ArgumentParser(description='Yolo')
        parser.add_argument('vidPath', type=str, help='Path to video')
        parser.add_argument('threadsCount', type=int, help='Count of worker threads')
        parser.add_argument('outputName', type=str, help='Name of output file')

        args = parser.parse_args()
        video_path = args.vidPath
        self.threads_count = args.threadsCount
        if self.threads_count < 1:
            print('Expected at least one worker thread')
            return
        output_name = args.outputName

        self.video = cv2.VideoCapture(video_path)
        fourcc = cv2.VideoWriter_fourcc(*'mp4v')
        self.writer = cv2.VideoWriter(output_name, fourcc, 20.0, (640, 480))

        self.stop_event = Event()

        self.task_queue = Queue()
        self.result_by_index = {}

    def run(self):
        processing_begin_time = time()

        threads = [Thread(target=self.worker, args=()) for _ in range(self.threads_count)]
        for thread in threads:
            thread.start()

        counter = 0
        while self.video.isOpened():
            ret, frame = self.video.read()
            if not ret:
                break
            self.task_queue.put((frame, counter))
            counter += 1

        while not self.task_queue.empty():
            sleep(0.01)
        self.stop_event.set()

        counter = 0
        while counter in self.result_by_index:
            self.writer.write(self.result_by_index[counter])
            counter += 1

        process_time = time() - processing_begin_time

        for thread in threads:
            thread.join()

        print(f'Processing took {process_time:.2f} seconds')

    def __del__(self):
        self.stop_event.set()
        self.writer.release()
        self.video.release()

    def worker(self):
        model = YOLO('yolov8s-pose.pt')

        while not self.stop_event.is_set():
            if self.task_queue.empty():
                sleep(0.01)
                continue

            (frame, index) = self.task_queue.get()
            annotated_frame = model.predict(frame, verbose=False, device='cpu')[0].plot()
            self.result_by_index[index] = annotated_frame
