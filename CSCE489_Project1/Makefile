CC=gcc
CFLAGS=-ggdb -Wall -Wextra -O0
DEPS = shellfuncts.h
OBJ = myshell.o shellfuncts.o -lm

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

myshell: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o *~ myshell 
