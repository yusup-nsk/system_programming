#include "common_data.h"
int main() {
  int sockfd;
  socklen_t clilen;
  int n;
  char buffer[BUFFER_SIZE];
  struct sockaddr_un servaddr, cliaddr;
  if ((sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0) {
    handle_error("socket");
  }
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sun_family = AF_LOCAL;
  strncpy(servaddr.sun_path, SERVER_SOCKET_NAME, sizeof(servaddr.sun_path) - 1);
  if (bind(sockfd, (struct sockaddr *)&servaddr, SUN_LEN(&servaddr)) < 0) {
    close(sockfd);
    handle_error("bind");
  }
  clilen = sizeof(struct sockaddr_un);
  if ((n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr,
                    &clilen)) < 0) {
    close(sockfd);
    handle_error("recvfrom");
  }
  buffer[n] = '\0';
  printf("\033[36m%s\033[0m\n", buffer);

  if (sendto(sockfd, "Hi!", 4, 0, (struct sockaddr *)&cliaddr, clilen) < 0) {
    close(sockfd);
    handle_error("sendto");
  }
  close(sockfd);
  unlink(SERVER_SOCKET_NAME);
   exit(EXIT_SUCCESS);
}