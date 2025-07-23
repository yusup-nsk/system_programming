CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic
CURSES_FLAG := -lncurses
EXECUTABLE := mycommander

run: exe
	./$(EXECUTABLE)
exe:
	$(CC) $(CFLAGS) main.c -o $(EXECUTABLE) $(CURSES_FLAG)

format:
	clang-format --style=Google -i *.[ch]