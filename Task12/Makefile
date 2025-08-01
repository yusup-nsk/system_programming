CC:=gcc
CFLAGS:=-Werror -Wextra -Wall -pedantic -g
EXECUTABLE:=shop
SOURCE:=quest3.c
VALGRIND_LOG:=valgrind.log
LIB_FLAG:=-pthread

all: exe run

exe:
	$(CC) $(CFLAGS) $(SOURCE) -o $(EXECUTABLE) $(LIB_FLAG)

run:
	./$(EXECUTABLE)

format:
	clang-format -i --style=Google *.c

analyz:
	cppcheck --enable=all --suppress=missingIncludeSystem $(SOURCE)

leaks:
	 valgrind --trace-children=yes --track-fds=yes --track-origins=yes \
	  --leak-check=full --show-leak-kinds=all -s --show-reachable=no --log-file="$(VALGRIND_LOG)" ./$(EXECUTABLE)
	tail -7 $(VALGRIND_LOG)

clean:
	rm -f *.log a.out *.o $(EXECUTABLE)

