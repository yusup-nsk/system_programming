
#ifndef _COMMON_DATA_H_
#define _COMMON_DATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#define MSG_LEN 255
#define NAME_LEN 30
#define CHAT_MSG_LEN NAME_LEN + 4 + MSG_LEN
#define PRIO_SERVER_TO_CLIENT 100
#define PRIO_CLIENT_TO_SERVER 300
#define FILE_TO_KEY "Makefile"
#define SYMBOL_CHAT 'C'
#define SYMBOL_NEW_NAME 'N'

#define NUMBER_TO_KEY 2000
#define MAX_CLIENTS 100
#define MAX_NUM_CHAT 200

struct msgbuff {
  long prioritet;
  char text_msg[MSG_LEN];
};
struct chat_msg {
  long prioritet;
  char text_msg[CHAT_MSG_LEN + 1];
};

int get_msqid(unsigned id);
int make_msqid(unsigned id);

#endif