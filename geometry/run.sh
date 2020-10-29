#!/bin/bash

set -e

g++ -std=c++17 -I./src test/test.cpp src/geometry.cpp -o geometry
./geometry

echo All tests passed!