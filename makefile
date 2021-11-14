GCC = gcc


SFML_OUT= sfml-app.out
# INPUT_FILE= main.cpp  Entity.cpp
FLAG_OPTI = -O3 -finline-functions -funroll-loops -march=native -mtune=native -flto
DEBUG_FLAG = -g #-fsanitize=address
LFLAGS= -lm #-lpthread

FLAG = $(FLAG_OPTI)

all : present24

##
main : src/main.c
	$(GCC) -c $(FLAG) "src/main.c" -o builds/main.out

##
sort : src/sort/sort.c
	$(GCC) -c $(FLAG) "src/sort/sort.c" -o builds/sort.out

##
keys : src/present/keys.c
	$(GCC) -c $(FLAG) "src/present/keys.c" -o builds/keys.out

encryption : src/present/encryption.c
	$(GCC) -c $(FLAG) "src/present/encryption.c" -o builds/encryption.out

decryption : src/present/decryption.c
	$(GCC) -c $(FLAG) "src/present/decryption.c" -o builds/decryption.out

##
attack : src/attack/attack.c
	$(GCC) -c $(FLAG) "src/attack/attack.c" -o builds/attack.out



present24 : main keys encryption decryption attack sort
	$(GCC) -o present.out $(FLAG) builds/main.out builds/keys.out builds/encryption.out builds/decryption.out builds/attack.out builds/sort.out  $(LFLAGS)


clear :
	rm *.out




