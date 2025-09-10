#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_SOCKET_NAME "local_dgram_server.socket"
#define CLIENT_SOCKET_NAME "local_dgram_client.socket"
#define BUFFER_SIZE 12
#define IP "127.0.0.1"
#define PORT 7777

#define handle_error(msg)                                           \
  do {                                                              \
    char str[100] = {0};                                            \
    sprintf(str, "%s:%d:%s:%s", __FILE__, __LINE__, __func__, msg); \
    perror(str);                                                    \
    exit(EXIT_FAILURE);                                             \
  } while (0)
