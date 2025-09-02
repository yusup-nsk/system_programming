CC:=gcc
CFLAGS:=-Werror -Wextra -Wall -pedantic -g -lrt
CLIENT_EXE:=client 
SERVER_EXE:=server
SOURCE_CLIENT:=client.c 
SOURCE_SERVER:=server.c
VALGRIND1_LOG:=valgrind1.log
VALGRIND2_LOG:=valgrind2.log


all: exe1 exe2 run

exe1:
	$(CC) $(CFLAGS) $(SOURCE_SERVER) -o $(SERVER_EXE) 
exe2:
	$(CC) $(CFLAGS) $(SOURCE_CLIENT) -o $(CLIENT_EXE) 

run:
	@printf "\nServer started. Run './client' in another terminal\n"
	./$(SERVER_EXE)

format:
	clang-format -i --style=Google *.c

analyz: 
	cppcheck --enable=all --suppress=missingIncludeSystem *.c

leaks: $(SERVER_EXE) $(CLIENT_EXE)
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes \
	  --leak-check=full --show-leak-kinds=all -s --show-reachable=no --log-file="$(VALGRIND1_LOG)" ./$(SERVER_EXE) &
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes \
	  --leak-check=full --show-leak-kinds=all -s --show-reachable=no --log-file="$(VALGRIND2_LOG)" ./$(CLIENT_EXE) &

clean:
	rm -f *.log a.out *.o $(SERVER_EXE) $(CLIENT_EXE) 

.PHONY: all run exe1 exe2 clean format analyz leaks

