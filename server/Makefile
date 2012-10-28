CC=gcc
CFLAGS=-g
DEPS = server.h  lib/udpOverTcp.h
NAME = server
OBJ = server.o main.o  lib/udpOverTcp.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
clean:
	rm -f *.o $(NAME)