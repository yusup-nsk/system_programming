

#include "common_data.h"

int main() {
  struct sockaddr_un server_addr;
  int ret;
  int data_socket;
  char buffer[BUFFER_SIZE] = {"Hello!\0"};
  data_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (data_socket == -1) handle_error("socket");

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_LOCAL;
  strncpy(server_addr.sun_path, SOCKET_NAME, sizeof(server_addr.sun_path) - 1);
  ret = connect(data_socket, (const struct sockaddr *)&server_addr,
                sizeof(server_addr));
  if (ret == -1) handle_error("connect");

  ret = send(data_socket, buffer, 1 + strlen(buffer), 0);
  if (ret < 0) handle_error("send");

  ret = recv(data_socket, buffer, BUFFER_SIZE, 0);
  if (ret < 0) handle_error("recv");

  buffer[sizeof(buffer) - 1] = 0;
  printf("\033[42m%s\033[0m\n", buffer);

  close(data_socket);
  exit(EXIT_SUCCESS);
}
