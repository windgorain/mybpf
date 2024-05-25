#!/bin/bash

mkdir build
cd build
cmake \
   -DCMAKE_BUILD_TYPE=Release \
   -DIN_LINUX=ON \
   -DIN_UNIXLIKE=ON \
   -DUSE_BACKTRACE=ON \
   ..

make -j

