from argparse import ArgumentParser

from ultralytics import YOLO


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

    # TODO: Implement
    pass


main()
