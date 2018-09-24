CXX=g++
CC=gcc
CFLAGS=-Wall -g
CXXFLAGS=-std=c++11 -D__STDC_CONSTANT_MACROS
LDFLAGS=-I./include -L./lib -lavcodec -lavformat -lavutil -lavfilter -lavdevice -lswresample -lpostproc -lswscale -lm
RM=rm -rf

all: server client test

server:
	$(CXX) $(CXXFLAGS) $(CFLAGS) src/server/*.cpp -o server_tcp $(LDFLAGS)

client:
	$(CXX) $(CXXFLAGS) $(CFLAGS) src/client/*.cpp -o client_tcp

test:
	$(CXX) $(CXXFLAGS) $(CFLAGS) src/demuxing_decoding.cpp -o test $(LDFLAGS)

clean:
	$(RM) server_tcp client_tcp test *.dSYM img_processed/output*
