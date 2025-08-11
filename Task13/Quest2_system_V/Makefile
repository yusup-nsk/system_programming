CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic -g
CURSES_FLAG := -lncurses
EXECUTABLE := ex

all: clean format exe run

run:
	./$(EXECUTABLE)

exe:
	$(CC) $(CFLAGS) -c functions/*.c  $(CURSES_FLAG)
	$(CC) $(CFLAGS) main.c *.o -o $(EXECUTABLE) $(CURSES_FLAG)

format:
	clang-format --style=Google -i *.[ch] functions/*.[ch]

analyz:
	cppcheck --enable=all --suppress=missingIncludeSystem *.[ch] functions/*.[ch]

cleano:
	rm -f *.o 

clean: cleano
	rm -f $(EXECUTABLE) *.log

.PHONY: all clean exe format cleano 