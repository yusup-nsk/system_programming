#include "common_data.h"

int main() {
  struct sockaddr_un server_address, client_address;
  int ret;
  int connection_socket;
  int data_socket;
  char buffer[BUFFER_SIZE];
  socklen_t client_addr_size;

  connection_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (connection_socket == -1) handle_error("socket");

  memset(&server_address, 0, sizeof(server_address));
  server_address.sun_family = AF_LOCAL;
  strncpy(server_address.sun_path, SOCKET_NAME,
          sizeof(server_address.sun_path) - 1);

  ret = bind(connection_socket, (struct sockaddr *)&server_address,
             sizeof(server_address));
  if (ret == -1) handle_error("bind");

  ret = listen(connection_socket, LISTEN_BACKLOG);
  if (ret == -1) handle_error("listen");

  client_addr_size = sizeof(client_address);
  data_socket = accept(connection_socket, (struct sockaddr *)&client_address,
                       &client_addr_size);
  if (data_socket == -1) handle_error("accept");

  ret = recv(data_socket, buffer, BUFFER_SIZE, 0);
  if (ret < 0) handle_error("recv");

  buffer[sizeof(buffer) - 1] = 0;
  printf("\033[36m%s\033[0m\n", buffer);

  strncpy(buffer, "Hi!", 4);
  ret = send(data_socket, buffer, 1 + strlen(buffer), 0);
  if (ret < 0) handle_error("send");

  sleep(2);
  close(connection_socket);
  unlink(SOCKET_NAME);

  exit(EXIT_SUCCESS);
}
