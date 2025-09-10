#include "common_data.h"

int main() {
  int sockfd;
  int len;
  char buffer[BUFFER_SIZE] = {"Hello!\0"};
  struct sockaddr_in servaddr;  // cliaddr;
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    handle_error("socket");
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  if (inet_aton(IP, &servaddr.sin_addr) == 0) {
    close(sockfd);
    handle_error("inet_aton - invalid IP address");
  }

  connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  if (send(sockfd, buffer, strlen(buffer) + 1, 0) < 0) {
    close(sockfd);
    handle_error("sendto");
  }

  if ((len = recv(sockfd, buffer, BUFFER_SIZE, 0)) < 0) {
    close(sockfd);
    handle_error("recvfrom");
  }
  buffer[len] = '\0';
  printf("\033[42m%s\033[0m\n", buffer);
  
  close(sockfd);
  exit(EXIT_SUCCESS);
}