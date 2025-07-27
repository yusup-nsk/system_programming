CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic
CURSES_FLAG := -lncurses
EXECUTABLE := mycommander

run: clean exe
	./$(EXECUTABLE)
exe:
	$(CC) $(CFLAGS) -c functions/*.c  $(CURSES_FLAG)
	$(CC) $(CFLAGS) main.c *.o -o $(EXECUTABLE) $(CURSES_FLAG)

format:
	clang-format --style=Google -i *.[ch] functions/*.[ch]

cleano:
	rm -f *.o 

clean: cleano
	rm -f $(EXECUTABLE) a.out