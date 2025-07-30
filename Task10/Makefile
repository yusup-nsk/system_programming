CC:=gcc
CFLAGS:=-Werror -Wextra -Wall -pedantic -g
EXECUTABLE:=mybash
SOURCE:=q3.c
#COMMAND:= ls -a -l ../
VALGRIND_LOG:=valgrind.log

all: exe run

exe:
	$(CC) $(CFLAGS) $(SOURCE) -o $(EXECUTABLE)

run:
	./$(EXECUTABLE) $(COMMAND)

format:
	clang-format -i *.c

analyz:
	cppcheck --enable=all --suppress=missingIncludeSystem $(SOURCE)

leaks:
	 valgrind --trace-children=yes --track-fds=yes --track-origins=yes \
	  --leak-check=full --show-leak-kinds=all -s --show-reachable=no --log-file="$(VALGRIND_LOG)" ./$(EXECUTABLE)
	tail -7 $(VALGRIND_LOG)
clean:
	rm -f *.log

