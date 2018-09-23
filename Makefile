CXX=g++
CC=gcc
CFLAGS=-Wall -g -I./include
CXXFLAGS=-std=c++11
LDFLAGS=-L./lib -lavcodec -lm
RM=rm -rf

all: server test

server:
	$(CXX) $(CXXFLAGS) $(CFLAGS) src/server/*.cpp -o server_tcp $(LDFLAGS)

test:
	$(CC) $(CFLAGS) src/*.c -o Test $(LDFLAGS)

clean:
	$(RM) server Test *.DSYM
