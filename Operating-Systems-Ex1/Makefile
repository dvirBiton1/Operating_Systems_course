.PHONY: all clean

all: main server
main: main.o server.o client.o
	gcc main.o client.o -o main
server: server.o
	gcc server.o -o server
main.o: Ex1.h main.c 
	gcc -c main.c 
server.o: server.c Ex1.h
	gcc -c server.c 
client.o: client.c Ex1.h
	gcc -c client.c 
clean :
	rm -f *.o server main