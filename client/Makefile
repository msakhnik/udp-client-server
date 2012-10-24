CC=gcc
CFLAGS=-g
DEPS = client.h unprtt
NAME = client
OBJ = client.o rtt.o main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
clean:
	rm -f *.o $(NAME)
