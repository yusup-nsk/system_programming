#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_SOCKET_NAME "local_dgram_server.socket"
#define CLIENT_SOCKET_NAME "local_dgram_client.socket"
#define BUFFER_SIZE 12

#define handle_error(msg)                              \
  do {                                                 \
    char str[100] = {0};                               \
    sprintf(str, "%s:%d:%s", __FILE__, __LINE__, msg); \
    perror(str);                                       \
    exit(EXIT_FAILURE);                                \
  } while (0)
