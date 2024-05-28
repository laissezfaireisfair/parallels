#!/bin/bash
set -euxo pipefail

pgc++ src/main.cpp -Wall -Wextra -Wpedantic -acc=host -Minfo=all -o host.out

pgc++ src/main.cpp -Wall -Wextra -Wpedantic -acc=multicore -Minfo=all -o multicore.out

pgc++ src/main.cpp -Wall -Wextra -Wpedantic -acc=gpu -Minfo=all -o gpu.out

for GRID_SIZE in 128 256 512 1024
do
  time ./host.out 0.0005 $GRID_SIZE 1000000 > /dev/null

  time ./multicore.out 0.0005 $GRID_SIZE 1000000 > /dev/null

  time ./gpu.out 0.0005 $GRID_SIZE 1000000 > /dev/null
done