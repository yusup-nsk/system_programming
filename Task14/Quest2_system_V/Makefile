CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic -g
CURSES_FLAG := -lncurses
SOURCE1 := server.c
EXECUTABLE1 := server
SOURCE2 := client.c
EXECUTABLE2 := client
COMMON_DATA_SERVER_AND_CLIENT := functions/common_data.c

all: exe1 exe2 
	@./$(EXECUTABLE1)

exe1:
	$(CC) $(CFLAGS) $(SOURCE1) $(COMMON_DATA_SERVER_AND_CLIENT) -o $(EXECUTABLE1) $(CURSES_FLAG) 
exe2:
	$(CC) $(CFLAGS) -c functions/*.c  $(CURSES_FLAG)
	$(CC) $(CFLAGS) $(SOURCE2) *.o -o $(EXECUTABLE2) $(CURSES_FLAG)

format:
	clang-format --style=Google -i *.[ch] functions/*.[ch]

analyz:
	cppcheck --enable=all --suppress=missingIncludeSystem *.[ch] functions/*.[ch]

cleano:
	rm -f *.o 

clean: cleano
	rm -f $(EXECUTABLE1) $(EXECUTABLE2) *.log

.PHONY: all exe1 exe2 clean format cleano 