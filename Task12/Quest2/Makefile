CC:=gcc
CFLAGS:=-Werror -Wextra -Wall -pedantic -g
CLIENT_EXE:=client 
SERVER_EXE:=server
SOURCE_CLIENT:=client.c 
SOURCE_SERVER:=server.c
PIPE_FILE:=pipe_server_to_client
VALGRIND1_LOG:=valgrind1.log
VALGRIND2_LOG:=valgrind2.log


all: exe run

exe: exe1 exe2

exe1:
	$(CC) $(CFLAGS) $(SOURCE_SERVER) -o $(SERVER_EXE) 
exe2:
	$(CC) $(CFLAGS) $(SOURCE_CLIENT) -o $(CLIENT_EXE) 

run:
	./$(SERVER_EXE) &
	sleep 3
	./$(CLIENT_EXE)

format:
	clang-format -i --style=Google *.c

analyz: 
	cppcheck --enable=all --suppress=missingIncludeSystem *.c

leaks: $(SERVER_EXE) $(CLIENT_EXE)
	make leaks1 
	make leaks2

leaks1: $(SERVER_EXE)
	 valgrind --trace-children=yes --track-fds=yes --track-origins=yes \
	  --leak-check=full --show-leak-kinds=all -s --show-reachable=no --log-file="$(VALGRIND1_LOG)" ./$(SERVER_EXE) &
leaks2: $(CLIENT_EXE)
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes \
	  --leak-check=full --show-leak-kinds=all -s --show-reachable=no --log-file="$(VALGRIND2_LOG)" ./$(CLIENT_EXE) &



clean:
	rm -f *.log a.out *.o $(SERVER_EXE) $(CLIENT_EXE) $(PIPE_FILE)

