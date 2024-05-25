#!/bin/bash

mkdir build
cd build
cmake \
   -DCMAKE_BUILD_TYPE=Release \
   -DIN_LINUX=OFF \
   -DIN_MAC=ON \
   -DIN_UNIXLIKE=ON \
   -DUSE_BACKTRACE=ON \
   ..

make -j

