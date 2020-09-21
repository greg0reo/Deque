CFLAGS= -Wall -Wextra -pedantic -ldl

all: test

test: test.o
	g++ $(CFLAGS) test.o -o test

test.o: test.cpp
	g++ -g -c test.cpp

clean:
	rm -f *.o temp

