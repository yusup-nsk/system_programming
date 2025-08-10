#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>

#define MSG_LEN 128
#define PRIORITY 8
#define N_ATTEMPTS 100

int main() {
  char mq_name[MSG_LEN] = "/message_queue";
  char str[MSG_LEN] = {0};
  unsigned prioritet;
  mqd_t mqdes = mq_open(mq_name, O_RDWR);
  for (int attempt = 0; attempt < N_ATTEMPTS && -1 == mqdes; attempt++) {
    usleep(10000);
    mqdes = mq_open(mq_name, O_RDWR);
  }
  if (-1 == mqdes) {
    perror("client openning message queue");
    exit(EXIT_FAILURE);
  }

  if (mq_receive(mqdes, str, MSG_LEN, &prioritet) == -1) {
    perror("client recieving");
    mq_close(mqdes);
    exit(EXIT_FAILURE);
  }
  str[MSG_LEN - 1] = '\0';
  printf("\n\033[1;32m%s\033[0m\n\n", str);

  if (mq_send(mqdes, "Hello!", 10, PRIORITY) == -1) {
    perror("sending client--->server");
    mq_close(mqdes);
    exit(EXIT_FAILURE);
  }

  mq_close(mqdes);
  exit(EXIT_SUCCESS);
}