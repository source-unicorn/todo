CC = gcc
CFLAGS = -Wall
LIBS = -lncurses

todo: todo.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

todo.o: todo.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f todo *.o
