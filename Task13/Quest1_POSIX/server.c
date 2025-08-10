#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
// #include <sys/types.h>
#include <sys/msg.h>
#define MSG_LEN 128
#define PRIORITY 8

struct msgbuff {
  long prioritet;
  char text_msg[MSG_LEN];
};

int main() {
  char filename[2 * MSG_LEN];
  //  char str[MSG_LEN];
  int msgid = 0;
  struct msgbuff message1 = {.prioritet = PRIORITY, "Hi!"}, message2;
  sprintf(filename, "%s/%s", getenv("PWD"), "Makefile");
  key_t key = ftok(filename, 2025);
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
    // printf("Failed to send message %s\n", message1.text_msg);
    perror("sending message");
    while (-1 == res_msgsnd && EAGAIN == errno) {
      sleep(1);
      res_msgsnd = msgsnd(msgid, &message1, sizeof(message1), IPC_NOWAIT);
    }
  }

  if (-1 == msgrcv(msgid, &message2, sizeof(struct msgbuff), PRIORITY + 1, 0)) {
    perror("Failed to recieve message\n");
  } else {
    printf("\033[1;36m%s\033[0m\n", message2.text_msg);
  }

  //   struct msqid_ds buf;
  // // msgctl(msgid, IPC_STAT, )
  // // msgctl(msgid, IPC_SET , )
  // msgctl(msgid, IPC_RMID, &buf);

  struct msqid_ds buf;
  msgctl(msgid, IPC_RMID, &buf);

  exit(EXIT_SUCCESS);
}