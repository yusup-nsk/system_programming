CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic -g
CURSES_FLAG := -lncurses
SOURCE1 := client.c
EXECUTABLE1 := client
SOURCE2 := server.c
EXECUTABLE2 := server
COMMON_DATA_SERVER_AND_CLIENT := functions/common_data.c

all:
	make run2
	make run1

run1: exe1
	./$(EXECUTABLE1)

server:
	make exe2
	./$(EXECUTABLE2)

run2: exe2
	./$(EXECUTABLE2) &

exe1:
	$(CC) $(CFLAGS) -c functions/*.c  $(CURSES_FLAG)
	$(CC) $(CFLAGS) $(SOURCE1) *.o -o $(EXECUTABLE1) $(CURSES_FLAG)

exe2:
	$(CC) $(CFLAGS) $(SOURCE2) $(COMMON_DATA_SERVER_AND_CLIENT) -o $(EXECUTABLE2) 

format:
	clang-format --style=Google -i *.[ch] functions/*.[ch]

analyz:
	cppcheck --enable=all --suppress=missingIncludeSystem *.[ch] functions/*.[ch]

cleano:
	rm -f *.o 

clean: cleano
	rm -f $(EXECUTABLE1) $(EXECUTABLE2) *.log

.PHONY: all clean exe format cleano 