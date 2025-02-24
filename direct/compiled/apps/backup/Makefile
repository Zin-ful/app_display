CC = gcc
CFLAGS = -I../../ -Wall -pthread -g
OBJ = paint.o ../../keyboard.o ../../windower.o ../../mouse.o ../../text_loader.o
OUT = paint

all: $(OUT)


paint:  $(OBJ)
	   $(CC) $(CFLAGS) $(OBJ) -o paint


paint.o: paint.c
	   $(CC) $(CFLAGS) -c paint.c ../../windower.c ../../windower.h ../../keyboard.c ../../keyboard.h ../../text_loader.c ../../text_loader.h

clean:
	   rm -f *.o paint
