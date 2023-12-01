CXX=g++
CFLAGS=-I/opt/homebrew/opt/eigen/include/eigen3
LIBS=-lm
OBJS=main.o
SRCS=main.cpp

all: main
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) $(OBJS) -o main

main: $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) $(SRCS) -c $(OBJS)