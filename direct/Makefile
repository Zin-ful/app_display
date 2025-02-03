CC = gcc
CFLAGS = -Wall -pthread

SRC = main.c mouse.c keyboard.c windower.c

OUT = winmngr

all: $(OUT)

$(OUT): $(SRC)
	   $(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	   rm -f $(OUT)
