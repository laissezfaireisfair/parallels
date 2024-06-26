from argparse import ArgumentParser
from logging import getLogger

logger = getLogger(__name__)


class ProgramArguments:
    def __init__(self):
        parser = ArgumentParser(description='Sensor')
        parser.add_argument('cameraName', type=str, help='System name of the camera')
        parser.add_argument('cameraResolution', type=str, help='Camera resolution')
        parser.add_argument('resultFrequency', type=str, help='Result update frequency')

        args = parser.parse_args()
        self.camera_name = args.cameraName
        self.camera_resolution = ProgramArguments.parse_resolution(args.cameraResolution)
        self.result_frequency = int(args.resultFrequency)

    @staticmethod
    def parse_resolution(resolution_string: str) -> tuple[int, int]:
        left, right = resolution_string.split('x')
        return int(left), int(right)

    def __str__(self):
        return f'{self.camera_name} {self.camera_resolution} {self.result_frequency}'
