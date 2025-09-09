#include "common_data.h"

int main() {
  int sockfd;
  int len;
  socklen_t clilen;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in servaddr, cliaddr;
  int newsockfd;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
   handle_error("socket");
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    close(sockfd);
    handle_error("bind");
  }
  if (listen(sockfd, LISTEN_BACKLOG) < 0) {
    close(sockfd);
    handle_error("listen");
  }

  clilen = sizeof(cliaddr);

  if ((newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen)) < 0) {
    close(sockfd);
    handle_error("accept");
  }
//   char ipaddr[100];
//   strcpy(ipaddr,inet_ntoa((cliaddr.sin_addr)));
//   printf("client addres is %s\n", ipaddr);

  len = read(newsockfd, buffer, BUFFER_SIZE);
    if (len < 0) {
    close(sockfd);
    close(newsockfd);
    handle_error("read");
  }
    buffer[len] = '\0';
    printf("\033[36m%s\033[0m\n", buffer);

    strncpy(buffer, "Hi!", 4);
    if ((write(newsockfd, buffer, strlen(buffer) + 1)) < 0) {
      close(sockfd);
      close(newsockfd);
      handle_error("write");
    }
  close(newsockfd);
  close(sockfd);
   exit(EXIT_SUCCESS);
}