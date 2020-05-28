#!/bin/bash

set -e

g++ --std=c++11 -I ./Eigen matrix_test.cpp -o matrix_test
