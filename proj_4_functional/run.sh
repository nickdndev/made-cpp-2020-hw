#!/bin/bash

set -e

g++ -std=c++17 -I./ src/main.cpp -o main_test
./main_test

echo All tests passed!