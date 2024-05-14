#!/bin/bash
set -euxo pipefail

VENV_EXEC=/mnt/ssd/Active-projects/devVenv/bin/python

for THREADS_COUNT in 1 2 4 7 8 16 20 40
do
  $VENV_EXEC main.py resources/cropped.mp4 $THREADS_COUNT output.mp4
done