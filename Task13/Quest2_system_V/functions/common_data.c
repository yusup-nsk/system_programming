
// #ifndef _COMMON_DATA_C_
// #define _COMMON_DATA_C_

#include "common_data.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/ipc.h>
// #include <sys/msg.h>
// #include <sys/types.h>

// #define MSG_LEN 255
// #define NAME_LEN 30
// #define CHAT_MSG_LEN NAME_LEN + 4 + MSG_LEN
// #define PRIO_SERVER_TO_CLIENT 100
// #define PRIO_CLIENT_TO_SERVER 300
// #define FILE_TO_KEY "Makefile"
// #define SYMBOL_CHAT 'C'
// #define SYMBOL_NEW_NAME 'N'

// #define NUMBER_TO_KEY 2000
// #define MAX_CLIENTS 100
// #define MAX_NUM_CHAT 200

// struct msgbuff {
//   long prioritet;
//   char text_msg[MSG_LEN];
// };
// struct chat_msg {
//   long prioritet;
//   char text_msg[CHAT_MSG_LEN + 1];
// };

// int get_msqid(unsigned id);
// int make_msqid(unsigned id);

int get_msqid(unsigned id) {
  int msqid;
  char filename[2 * MSG_LEN];
  sprintf(filename, "%s/%s", getenv("PWD"), FILE_TO_KEY);
  key_t key = ftok(filename, id);
  if (-1 == key) {
    printf("--------- get-msqid key=-1 ------ \n");
    return -2;
  }
  if (key && (msqid = msgget(key, 0)) == -1) {
    printf("--------- get-msqid msqid = -1   ------ \n");
    return -1;
  }
  return msqid;
}

int make_msqid(unsigned id) {
  int msqid = 0;
  char filename[2 * MSG_LEN];
  sprintf(filename, "%s/%s", getenv("PWD"), FILE_TO_KEY);
  key_t key = ftok(filename, id);
  if (-1 == key) {
    printf("++++ key=-1 ++++\n");
    return -2;
  }
  if (key && (msqid = msgget(key, IPC_CREAT | 0666)) == -1) {
    printf("++++ mdgget=-1 ++++\n");
    return -1;
  }
  struct msgbuff msg;
  while (-1 != msgrcv(msqid, &msg, sizeof(msg), 0, IPC_NOWAIT))
    printf("++++ trying to clean queue ++++\n");
  ;
  return msqid;
}

// #endif