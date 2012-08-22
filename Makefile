
all: zmqcat

clean:
	rm -f zmqcat
	rm -f zmqcat.o

zmqcat: zmqcat.o
	gcc -std=c99 -o zmqcat zmqcat.o -lzmq

zmqcat.o: zmqcat.c
	gcc -std=c99 -c zmqcat.c

