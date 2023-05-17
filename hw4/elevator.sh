#!/bin/bash
g++ -std=c++14 -o elevator.o elevator.cpp;

./elevator.o | tee elevator.txt;

rm *.o;