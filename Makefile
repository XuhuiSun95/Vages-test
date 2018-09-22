CC=gcc
CFLAGS=-Wall -g -I./include
LDFLAGS=-L./lib -lavcodec -lm

all:
	$(CC) $(CFLAGS) src/*.c -o Test $(LDFLAGS)

clean:
	$(RM) Test
