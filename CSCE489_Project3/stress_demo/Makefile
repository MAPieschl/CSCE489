CC=g++
CXXFLAGS=-g -Wall -Wextra -O0 -pthread
DEPS = 
OBJ = stress_demo.o
BIN = stress_demo

%.o: %.cpp $(DEPS)
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ 

.PHONY: clean

clean:
	rm -f *.o *~ $(BIN)
