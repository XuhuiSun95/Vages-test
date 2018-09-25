CXX=g++
CC=gcc
CFLAGS=-Wall -g
CXXFLAGS=-std=c++11 -D__STDC_CONSTANT_MACROS
LDFLAGS=-I./include -L./lib -lavcodec -lavformat -lavutil -lm
RM=rm -rf

all: server client

server:
	$(CXX) $(CXXFLAGS) src/server/*.cpp -o server_tcp $(LDFLAGS)

client:
	$(CXX) $(CXXFLAGS) src/client/*.cpp -o client_tcp

clean:
	$(RM) server_tcp client_tcp *.dSYM img_processed/output* *.pgm
