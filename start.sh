#!/bin/bash
make clean
cmake .
make
./Supervisor 3 "/home/blt/CLionProjects/tredmred/Partner"