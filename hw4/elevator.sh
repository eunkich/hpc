#!/bin/bash
g++ -std=c++14 -o elevator.o elevator.cpp;

./elevator.o 100 10 | tee elevator.txt;

rm *.o;