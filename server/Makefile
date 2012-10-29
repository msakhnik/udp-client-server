all : server sniff
NAME=server sniff
CC=gcc
CFLAGS=-g  -lpcap
server: server.o
	gcc -o server server.o main.o -g
server.o: server.c server.h
	gcc -c  server.h server.c main.c
sniff: sniff.o
	gcc -o sniff sniff.o -lpcap
sniff.o: sniff.c
	gcc -c sniff.c -lpcap
clean:
	rm -f *.o $(NAME)