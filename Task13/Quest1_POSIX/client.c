#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>

#define LENGTH 128
#define MSG_LEN 128
#define PRIORITY 8

struct msgbuff {
  long prioritet;
  char text_msg[MSG_LEN];
};

int main() {
  char filename[2 * MSG_LEN];
  int msgid = 0;
  sprintf(filename, "%s/%s", getenv("PWD"), "Makefile");
  struct msgbuff message = {0};

  key_t key = ftok(filename, 2025);
  if (-1 == key) {
    perror("function ftok");
    exit(EXIT_FAILURE);
  }
  if (key && (msgid = msgget(key, 0)) == -1) {
    perror("get message");
    exit(EXIT_FAILURE);
  }

  if (-1 == msgrcv(msgid, &message, sizeof(struct msgbuff), PRIORITY, 0)) {
    perror("Failed to recieve message\n");
  } else {
    printf("\033[1;32m%s\033[0m\n", message.text_msg);
  }

  message.prioritet = PRIORITY + 1;
  strncpy(message.text_msg, "Hello!", MSG_LEN);
  message.text_msg[MSG_LEN - 1] = '\0';

  int res_msgsnd = msgsnd(msgid, &message, sizeof(message), IPC_NOWAIT);
  if (res_msgsnd == -1) {
    perror("sending message");
    while (-1 == res_msgsnd && EAGAIN == errno) {
      sleep(1);
      res_msgsnd = msgsnd(msgid, &message, sizeof(message), IPC_NOWAIT);
    }
  }

  exit(EXIT_SUCCESS);
}