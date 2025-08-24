
#include "common_data.h"

int get_msqid(unsigned id) {
  int msqid;
  char filename[2 * MSG_LEN];
  sprintf(filename, "%s/%s", getenv("PWD"), FILE_TO_KEY);
  key_t key = ftok(filename, id);
  if (-1 == key) {
    return -2;
  }
  if (key && (msqid = msgget(key, 0)) == -1) {
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
    return -2;
  }
  if (key && (msqid = msgget(key, IPC_CREAT | 0666)) == -1) {
    return -1;
  }
  struct msgbuff msg;
  while (-1 != msgrcv(msqid, &msg, sizeof(msg), 0, IPC_NOWAIT))
    ;
  return msqid;
}
