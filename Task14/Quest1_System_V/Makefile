CC:=gcc
CFLAGS:=-Werror -Wextra -Wall -pedantic -g
CLIENT_EXE:=client 
SERVER_EXE:=server
SOURCE_CLIENT:=client.c 
SOURCE_SERVER:=server.c

all: exe1 exe2 
	@printf "\nServer programm is running here.\nRun './client' in another terminal\n\n"
	@./$(SERVER_EXE)
	
exe1:
	@$(CC) $(CFLAGS) $(SOURCE_SERVER) -o $(SERVER_EXE) 
exe2:
	@$(CC) $(CFLAGS) $(SOURCE_CLIENT) -o $(CLIENT_EXE) 

format:
	clang-format -i --style=Google *.c

analyz: 
	cppcheck --enable=all --suppress=missingIncludeSystem *.c

clean:
	rm -f *.log a.out *.o $(SERVER_EXE) $(CLIENT_EXE)

.PHONY: all exe1 exe2 clean format analyz