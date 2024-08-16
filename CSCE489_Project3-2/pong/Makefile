CC=g++
CXXFLAGS=-g -Wall -Wextra -O0 -pthread
DEPS = animation.h
OBJ = pong.o animation.o
BIN = pong

%.o: %.cpp $(DEPS)
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ 

.PHONY: clean

clean:
	rm -f *.o *~ $(BIN)
