#include "common_data.h"

int main() {
  int sockfd;
  int len;
  char buffer[BUFFER_SIZE] = {"Hello!\0"};
  struct sockaddr_in servaddr, cliaddr;
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    handle_error("socket");
  }

  memset(&cliaddr, 0, sizeof(cliaddr));
  cliaddr.sin_family = AF_INET;
  cliaddr.sin_port = htons(0);
  cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0) {
    close(sockfd);
    handle_error("bind");
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  if (inet_aton(IP, &servaddr.sin_addr) == 0) {
    close(sockfd);
    handle_error("inet_aton - invalid IP address");
  }

  if (sendto(sockfd, buffer, strlen(buffer) + 1, 0,
             (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    close(sockfd);
    handle_error("sendto");
  }

  if ((len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)NULL,
                      NULL)) < 0) {
    close(sockfd);
    handle_error("recvfrom");
  }
  buffer[len] = '\0';
  printf("\033[42m%s\033[0m\n", buffer);
  close(sockfd);
   exit(EXIT_SUCCESS);
}