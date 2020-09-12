CFLAGS= -Wall -Wextra -pedantic -ldl

all: test

new: new.o
	g++ $(CFLAGS) new.o -o new

new.o: new.cpp
	g++ -g -c new.cpp




temp: temp.o
	g++ $(CFLAGS) temp.o -o temp

temp.o: temp.cpp
	g++ -g -c temp.cpp

test: test.o
	g++ $(CFLAGS) test.o -o test

test.o: test.cpp
	g++ -g -c test.cpp

clean:
	rm -f *.o test
