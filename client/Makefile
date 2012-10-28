CC=gcc
CFLAGS=-g
DEPS = client.h lib/udpOverTcp.h
NAME = client
OBJ = client.o main.o lib/udpOverTcp.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
clean:
	rm -f *.o $(NAME)