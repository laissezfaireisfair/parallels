#!/bin/bash
set -euxo pipefail

BUILD_DIR=cmake-build-debug

mkdir $BUILD_DIR -p

cd $BUILD_DIR || exit

cmake -G Ninja ..

ninja

for THREADS_COUNT in 1 2 4 7 8 16 20 40
do
  time ./Task2_2 $THREADS_COUNT
done