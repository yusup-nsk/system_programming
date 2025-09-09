CC:=gcc
CFLAGS:=-Werror -Wextra -Wall -pedantic -g
CLIENT_EXE:=client 
SERVER_EXE:=server
SOURCE_CLIENT:=client.c 
SOURCE_SERVER:=server.c
VALGRIND1_LOG:=valgrind1.log
VALGRIND2_LOG:=valgrind2.log

all: clean exe1 exe2
	@printf "\nServer started. Run './client' in another terminal\n"
	@./$(SERVER_EXE)
	
exe1:
	@$(CC) $(CFLAGS) $(SOURCE_SERVER) -o $(SERVER_EXE) 
exe2:
	@$(CC) $(CFLAGS) $(SOURCE_CLIENT) -o $(CLIENT_EXE) 

format:
	clang-format -i --style=Google *.c *.h

analyz: 
	cppcheck --enable=all --suppress=missingIncludeSystem *.c

leaks1: $(SERVER_EXE)
	 valgrind --trace-children=yes --track-fds=yes --track-origins=yes \
	  --leak-check=full --show-leak-kinds=all -s --show-reachable=no --log-file="$(VALGRIND1_LOG)" ./$(SERVER_EXE) &
leaks2: $(CLIENT_EXE)
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes \
	  --leak-check=full --show-leak-kinds=all -s --show-reachable=no --log-file="$(VALGRIND2_LOG)" ./$(CLIENT_EXE) &

clean:
	rm -f *.log a.out  $(SERVER_EXE) $(CLIENT_EXE) *.socket

.PHONY:  exe1 exe2 clean format leaks1 leaks2