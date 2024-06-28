all: hello_c

hello_c: hello.o
	gcc -o hello_c hello.o
hello.o: hello.c
	gcc -c hello.c
clean:
	rm hello.o hello_c
