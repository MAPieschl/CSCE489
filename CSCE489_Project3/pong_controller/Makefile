CC=g++
CXXFLAGS=-g -Wall -Wextra -O0 -pthread
DEP = ../rt_demo/rt_demo.h
OBJ = pong_controller.o
BIN = pong_controller

%.o: %.cpp $(DEPS)
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ 

.PHONY: clean

clean:
	rm -f *.o *~ $(BIN)
