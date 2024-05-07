from pathlib import Path
import logging
from time import sleep
import threading
logger = logging.getLogger(__name__)


def job():
    logger.info("Job started")
    sleep(0.5)
    logger.info("Job finished")


def main():
    Path("logs/").mkdir(exist_ok=True)
    logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s',
                        datefmt='%d-%m-%y %H:%M:%S', filename='logs/info_plus.log', filemode='a')
    logger.info("Start up")

    thread = threading.Thread(target=job)
    thread.start()

    logger.info("Now waiting for thread to finish")

    thread.join()
    logging.info("Finish")


main()
