
#include "common_data.h"

int main() {
  int sockfd;
  int len;
  socklen_t clilen;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in servaddr, cliaddr;

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    handle_error("socket");
  }
  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    close(sockfd);
    handle_error("bind");
  }

  clilen = sizeof(cliaddr);
  if ((len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                      (struct sockaddr *)&cliaddr, &clilen)) < 0) {
    close(sockfd);
    handle_error("recvfrom");
  }
  buffer[len] = '\0';
  printf("\033[36m%s\033[0m\n", buffer);

  strncpy(buffer, "Hi!", 4);
  if (sendto(sockfd, buffer, 1 + strlen(buffer), 0, (struct sockaddr *)&cliaddr,
             clilen) < 0) {
    close(sockfd);
    handle_error("sendto");
  }

  exit(EXIT_SUCCESS);
}