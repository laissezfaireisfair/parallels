from argparse import ArgumentParser

import cv2
from ultralytics import YOLO
from ultralytics.engine.results import Keypoints


def main():
    parser = ArgumentParser(description='Yolo')
    parser.add_argument('vidPath', type=str, help='Path to video')
    parser.add_argument('mode', type=str, help='Single-threaded or multi-threaded')
    parser.add_argument('outputName', type=str, help='Name of output file')

    args = parser.parse_args()
    video_path = args.vidPath
    mode = args.mode
    output_name = args.outputName

    model = YOLO('yolov8s-pose.pt')

    video = cv2.VideoCapture(video_path)

    while video.isOpened():
        _, frame = video.read()
        cv2.imshow('frame', frame)
        keypoints: Keypoints = model(frame)[0].keypoints
        print(keypoints.data)
        if cv2.waitKey(1) == ord('q'):
            break

    video.release()
    cv2.destroyAllWindows()


main()
