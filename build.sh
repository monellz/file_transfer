#!/usr/bash
rm -rf build

mkdir build
cd build
cmake ..
make

cd server
./server
