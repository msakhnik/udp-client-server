CC=gcc
CFLAGS=-g
DEPS = client.h
NAME = client
OBJ = client.o main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
clean:
	rm -f *.o $(NAME)