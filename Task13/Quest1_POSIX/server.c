/* Из-за того, что по условию задания очередь одна
и если  клиент не успевает считать сообщение, то сервер считывает свое же
сообщение. В этом случае сервер повторно отправляет сообщение */
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define MSG_LEN 128
#define PRIORITY 8
#define MAX_MESSAGES 10
#define SEND_MESSAGE                                  \
  if (mq_send(mqdes, hi, MSG_LEN, prioritet) == -1) { \
    perror("sending server-->client");                \
    mq_close(mqdes);                                  \
    exit(EXIT_FAILURE);                               \
  }

int main() {
  char mq_name[MSG_LEN] = "/message_queue";
  char hi[] = "Hi!";
  unsigned prioritet = PRIORITY;
  struct mq_attr attributes = {.mq_maxmsg = MAX_MESSAGES,
                               .mq_msgsize = MSG_LEN};

  mqd_t mqdes = mq_open(mq_name, O_CREAT | O_RDWR, 0600, &attributes);
  if (-1 == mqdes) {
    perror("server open message queue");
    exit(EXIT_FAILURE);
  }

  SEND_MESSAGE

  pid_t pid = fork();
  if (0 == pid) {
    char str[MSG_LEN] = {0};
    int red_own_message = 1;
    do {
      sleep(1);
      if (mq_receive(mqdes, str, MSG_LEN, &prioritet) == -1) {
        perror("client recieving");
        mq_close(mqdes);
        exit(EXIT_FAILURE);
      }
      str[MSG_LEN - 1] = '\0';
      red_own_message = strncmp(str, hi, MSG_LEN) == 0;
      if (red_own_message) {
        SEND_MESSAGE
      }
    } while (red_own_message);
    printf("\n\n\033[1;36m%s\033[0m\n\n", str);
  } else {
    wait(NULL);
    kill(pid, SIGKILL);
    mq_close(mqdes);
    mq_unlink(mq_name);
  }
  exit(EXIT_SUCCESS);
}