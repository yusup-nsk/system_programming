#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/af_local_sock_stream.socket"
#define BUFFER_SIZE 12
#define LISTEN_BACKLOG 5

#define handle_error(msg)                              \
  do {                                                 \
    char str[100] = {0};                               \
    sprintf(str, "%s:%d:%s", __FILE__, __LINE__, msg); \
    perror(str);                                       \
    exit(EXIT_FAILURE);                                \
  } while (0)
