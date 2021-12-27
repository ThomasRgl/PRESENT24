#!/bin/bash

# gcc  main.c -o main.out -lm -lpthread
# ./main.out data/gcc.txt


# gcc  main.c -O3 -finline-functions -funroll-loops -march=native -mtune=native -flto -o main.out -lm -lpthread
# ./main.out data/gcc0.txt


# clang  main.c -o main.out -lm -lpthread
# ./main.out data/clang.txt


clang  main.c -O3 -finline-functions -funroll-loops -march=native -mtune=native -flto -o main.out -lm -lpthread
./main.out data/clang_div.txt


gnuplot a.gplot