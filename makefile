GCC = gcc


SFML_OUT= sfml-app.out
# INPUT_FILE= main.cpp  Entity.cpp
FLAG_OPTI = -O3 -finline-functions -funroll-loops -march=native -mtune=native -flto
DEBUG_FLAG = -g #-fsanitize=address
LFLAGS= -lm #-lpthread

FLAG = $(DEBUG_FLAG)

all : present24

##
main : src/main.c
	$(GCC) -c $(FLAG) "src/main.c" -o builds/main.out

##
keys : src/present/keys.c
	$(GCC) -c $(FLAG) "src/present/keys.c" -o builds/keys.out

encryption : src/present/encryption.c
	$(GCC) -c $(FLAG) "src/present/encryption.c" -o builds/encryption.out

decryption : src/present/decryption.c
	$(GCC) -c $(FLAG) "src/present/decryption.c" -o builds/decryption.out

##
#attack.out : src/attack/decryption.c
#	$(CC) -c $(FLAG) "src/present/decryption.c" -o builds/decryption.out



present24 : main keys encryption decryption
	$(GCC) -o test.out $(FLAG) builds/main.out builds/keys.out builds/encryption.out builds/decryption.out  $(LFLAGS)


clear :
	rm *.out




