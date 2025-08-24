#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h> 
#include <sys/stat.h>
#include <unistd.h>

#define MSG_LEN 128
#define PRIO_SERVER_TO_CLIENT 100
#define PRIO_CLIENT_TO_SERVER 300
#define FILE_TO_KEY "Makefile"
#define NUMBER_TO_KEY 2025

struct msgbuff {
  long prioritet;
  char text_msg[MSG_LEN];
};

int main() {
  char filename[2 * MSG_LEN];
  int msgid = 0;
  sprintf(filename, "%s/%s", getenv("PWD"), FILE_TO_KEY);
  struct msgbuff message = {0};

  key_t key = ftok(filename, NUMBER_TO_KEY);
  if (-1 == key) {
    perror("function ftok");
    exit(EXIT_FAILURE);
  }
  if (key && (msgid = msgget(key, 0)) == -1) {
    perror("get message");
    exit(EXIT_FAILURE);
  }

  if (-1 == msgrcv(msgid, &message, sizeof(struct msgbuff),
                   PRIO_SERVER_TO_CLIENT, 0)) {
    perror("Failed to recieve message\n");
  } else {
    printf("\033[1;32m%s\033[0m\n", message.text_msg);
  }

  message.prioritet = PRIO_CLIENT_TO_SERVER;
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