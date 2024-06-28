
all: hello_cpp

hello_cpp: hello_cpp.o
	g++ -o hello_cpp hello_cpp.o
hello_cpp.o: hello.cpp
	g++ -c hello.cpp -o hello_cpp.o
clean:
	rm hello_cpp.o hello_cpp
