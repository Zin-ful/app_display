CC = gcc
CFLAGS = -Wall -pthread

OUT = winmngr

all: $(OUT)

winmngr: main.o windower.o mouse.o keyboard.o
	   $(CC) $(CFLAGS) main.o windower.o mouse.o keyboard.o -o winmngr


main.o: main.c windower.c mouse.c keyboard.c
	   $(CC) $(CFLAGS) -c main.c

keyboard.o: keyboard.c keyboard.h
	   $(CC) $(CFLAGS) -c keyboard.c

windower.o: windower.c windower.h
	   $(CC) $(CFLAGS) -c windower.c

mouse.o: mouse.c mouse.h windower.c windower.h
	   $(CC) $(CFLAGS) -c mouse.c

clean:
	   rm -f *.o winmngr
