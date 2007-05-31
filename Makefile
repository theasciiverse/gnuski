CC=gcc
FLAGS=-O2 -o gnuski
LIBS=-lncurses
FILES=main.c objects.c

all:
	$(CC) $(FLAGS) $(FILES) $(LIBS)

clean:
	rm *~ gnuski
