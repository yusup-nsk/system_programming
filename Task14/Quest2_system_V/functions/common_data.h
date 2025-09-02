
#ifndef _COMMON_DATA_H_
#define _COMMON_DATA_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
//
#include <ncurses.h>

#define FILE_TO_KEY "Makefile"
#define FILE_TO_KEY_CLIENT_IDS "server.c"
#define FILE_TO_KEY_NAMES "server.c"
#define FILE_TO_KEY_CHAT (FILE_TO_KEY)
#define NUMBER_TO_KEY_CLIENT_IDS 'I'
#define NUMBER_TO_KEY_NAMES 'N'
#define NUMBER_TO_KEY_CHAT 'C'
#define NUMBER_TO_KEY 'K'

#define NUMBER_TO_KEY_SEM_NAMES 'S'
#define NUMBER_TO_KEY_SEM_CHAT '$'

#define MSG_LEN 255
#define NAME_LEN 30
#define CHAT_MSG_LEN (NAME_LEN + 6 + MSG_LEN)

#define MAX_CLIENTS 4
#define MAX_NUM_CHAT 6
#define SYMBOL_EXIT 'q'
#define SYMBOL_SEND_MESSAGE '!'
#define SYMBOL_WRITING_MESSAGE '_'
#define ESCAPE_KEY 27

typedef struct {
  char chat[MAX_NUM_CHAT][CHAT_MSG_LEN];
  unsigned last;
  int overflowed;  // при превышении MAX_NUM_CHAT новые сообщения записыавются
                   // на место старых
} ChatData;

union semun {
  int val;               /* value for SETVAL */
  struct semid_ds *buf;  /* buffer for IPC_STAT, IPC_SET */
  unsigned short *array; /* array for GETALL, SETALL */
                         /* Linux specific part: */
  struct seminfo *__buf; /* buffer for IPC_INFO */
};

void error_handle(int res, const char *msg);
int make_shared_memory(int *shmid, void **shmaddr, unsigned size,
                       char *file_to_key, unsigned number_to_key);
#endif