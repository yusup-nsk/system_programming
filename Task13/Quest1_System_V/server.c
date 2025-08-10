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
#define PRIO_CLIENT_TO_SERVER 200
#define FILE_TO_KEY "Makefile"
#define NUMBER_TO_KEY 2025

struct msgbuff {
  long prioritet;
  char text_msg[MSG_LEN];
};

int main() {
  char filename[2 * MSG_LEN];
  int msgid = 0;
  struct msgbuff message1 = {0}, message2 = {0};
  message1.prioritet = PRIO_SERVER_TO_CLIENT;
  strncpy(message1.text_msg, "Hi!", MSG_LEN - 1);
  sprintf(filename, "%s/%s", getenv("PWD"), FILE_TO_KEY);
  key_t key = ftok(filename, NUMBER_TO_KEY);
  if (-1 == key) {
    perror("creating key");
    exit(EXIT_FAILURE);
  }
  if ((msgid = msgget(key, IPC_CREAT | 0600)) == -1) {
    perror("getting message");
    exit(EXIT_FAILURE);
  }

  int res_msgsnd = msgsnd(msgid, &message1, sizeof(message1), IPC_NOWAIT);
  if (res_msgsnd == -1) {
    perror("sending message");
    while (-1 == res_msgsnd && EAGAIN == errno) {
      sleep(1);
      res_msgsnd = msgsnd(msgid, &message1, sizeof(message1), IPC_NOWAIT);
    }
  }

  if (-1 == msgrcv(msgid, &message2, sizeof(struct msgbuff),
                   PRIO_CLIENT_TO_SERVER, 0)) {
    perror("Failed to recieve message\n");
  } else {
    printf("\033[1;36m%s\033[0m\n", message2.text_msg);
  }

  struct msqid_ds buf;
  msgctl(msgid, IPC_RMID, &buf);

  exit(EXIT_SUCCESS);
}