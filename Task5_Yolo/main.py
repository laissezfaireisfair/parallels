from argparse import ArgumentParser

import cv2
from ultralytics import YOLO
from time import time


def main():
    parser = ArgumentParser(description='Yolo')
    parser.add_argument('vidPath', type=str, help='Path to video')
    parser.add_argument('mode', type=str, help='Single-threaded or multi-threaded')
    parser.add_argument('outputName', type=str, help='Name of output file')

    args = parser.parse_args()
    video_path = args.vidPath
    mode = args.mode
    if mode != 'singlethread':
        print('Unknown mode. Expected singlethread Exiting...')
        return
    output_name = args.outputName

    model = YOLO('yolov8s-pose.pt')

    video = cv2.VideoCapture(video_path)
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    writer = cv2.VideoWriter(output_name, fourcc, 20.0, (640, 480))

    processing_begin_time = time()

    while video.isOpened():
        ret, frame = video.read()
        if not ret:
            print('Cannot read frame. Exiting...')
            break
        annotated_frame = model(frame)[0].plot()
        writer.write(annotated_frame)

    process_time = time() - processing_begin_time

    writer.release()
    video.release()
    cv2.destroyAllWindows()

    print(f'Processing took {process_time:.2f} seconds')


main()
