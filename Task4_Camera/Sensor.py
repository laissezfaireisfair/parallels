from logging import getLogger

logger = getLogger(__name__)


class Sensor:
    def __init__(self, is_valid_child: bool = False):
        if not is_valid_child:
            raise RuntimeError('Cannot instantiate abstract class Sensor')
