CC = gcc
CFLAGS = -I../../ -Wall -pthread
OBJ = test.o ../../keyboard.o ../../windower.o ../../mouse.o ../../text_loader.o
OUT = test

all: $(OUT)


test:  $(OBJ)
	   $(CC) $(CFLAGS) $(OBJ) -o test


test.o: test.c
	   $(CC) $(CFLAGS) -c test.c ../../windower.c ../../windower.h ../../keyboard.c ../../keyboard.h ../../text_loader.c ../../text_loader.h
clean:
	   rm -f *.o test
