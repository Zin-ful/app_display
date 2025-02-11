CC = gcc
CFLAGS = -Wall -pthread

OUT = winmngr

all: $(OUT)

winmngr: main.o windower.o mouse.o keyboard.o text_loader.o
	   $(CC) $(CFLAGS) main.o windower.o mouse.o keyboard.o text_loader.o -o winmngr


main.o: main.c windower.c mouse.c keyboard.c text_loader.c
	   $(CC) $(CFLAGS) -c main.c

keyboard.o: keyboard.c keyboard.h text_loader.c text_loader.h windower.c windower.h
	   $(CC) $(CFLAGS) -c keyboard.c

windower.o: windower.c windower.h main.c main.h
	   $(CC) $(CFLAGS) -c windower.c

mouse.o: mouse.c mouse.h windower.c windower.h
	   $(CC) $(CFLAGS) -c mouse.c
text_loader.o: text_loader.c text_loader.h windower.c windower.h
	   $(CC) $(CFLAGS) -c text_loader.c

clean:
	   rm -f *.o winmngr
