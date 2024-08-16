CC=g++
CXXFLAGS=-g -Wall -Wextra -O0 -pthread
DEPS = rt_demo.h
OBJ = rt_demo.o
BIN = rt_demo

%.o: %.cpp $(DEPS)
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ 

.PHONY: clean

clean:
	rm -f *.o *~ $(BIN)
