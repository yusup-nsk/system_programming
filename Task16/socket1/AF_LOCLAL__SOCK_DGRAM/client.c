#include "common_data.h"
int main() {
  int sockfd;
  int len;
  char buffer[BUFFER_SIZE] = {"Hello!\0"};
  struct sockaddr_un servaddr, cliaddr;
  socklen_t socklen;
  if ((sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0) {
    handle_error("socket");
  }
  memset(&cliaddr, 0, sizeof(cliaddr));
  cliaddr.sun_family = AF_LOCAL;
  strncpy(cliaddr.sun_path, CLIENT_SOCKET_NAME, sizeof(cliaddr.sun_path) - 1);

  socklen = SUN_LEN(&cliaddr);
  if (bind(sockfd, (struct sockaddr *)&cliaddr, socklen) < 0) {
    close(sockfd);
    handle_error("bind");
  }
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sun_family = AF_LOCAL;
  strncpy(servaddr.sun_path, SERVER_SOCKET_NAME, sizeof(servaddr.sun_path) - 1);
  if (sendto(sockfd, buffer, strlen(buffer) + 1, 0,
             (struct sockaddr *)&servaddr, socklen) < 0) {
    close(sockfd);
    handle_error("sendto");
  }
  if ((len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                    (struct sockaddr *)&servaddr, &socklen)) < 0) {
    close(sockfd);
    handle_error("recvfrom");
  }
  buffer[len] = 0;
  printf("\033[42m%s\033[0m\n", buffer);

  close(sockfd);
  unlink(CLIENT_SOCKET_NAME);
   exit(EXIT_SUCCESS);
}