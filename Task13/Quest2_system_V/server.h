#include <errno.h>
#include <fcntl.h>
#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "functions/common_data.h"

#define SYMBOL_EXIT 'q'

typedef struct {
  unsigned id;
  char name[NAME_LEN];
  int mqueue;
  int condition;
} Client;

typedef struct {
  Client clients[MAX_CLIENTS];
  unsigned size_clients;
  char chat[MAX_NUM_CHAT][CHAT_MSG_LEN];
  unsigned size_chat;
  char message[MSG_LEN];  // ???
  int attribute;          // ???
} Information;
