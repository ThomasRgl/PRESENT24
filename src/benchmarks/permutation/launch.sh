#!/bin/bash

gcc  main.c -o main.out
./main.out data/gcc.txt



gcc  main.c -O3 -finline-functions -funroll-loops -march=native -mtune=native -flto -o main.out
./main.out data/gcc0.txt


clang  main.c -o main.out
./main.out data/clang.txt


clang  main.c -O3 -finline-functions -funroll-loops -march=native -mtune=native -flto -o main.out
./main.out data/clang0.txt


gnuplot a.gplot