#include "common_data.h"

void error_handle(int res, const char *msg) {
  if (res) {
    char str[CHAT_MSG_LEN];
    sprintf(str, "(pid #%d): %s", getpid(), msg);
    perror(str);
    exit(1);
  }
}

int make_shared_memory(int *shmid, void **shmaddr, unsigned size,
                       char *file_to_key, unsigned number_to_key) {
  char filename[2 * MSG_LEN];
  sprintf(filename, "%s/%s", getenv("PWD"), file_to_key);
  key_t key = ftok(filename, number_to_key);
  if (-1 == key) {
    perror("creating key");
    return -1;
  }
  if ((*shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0660)) == -1) {
    if (errno != EEXIST) {
      perror("getting shmget");
      return -2;
    } else {
      if ((*shmid = shmget(key, size, 0)) == -1) {
        perror("shmget error");
        return -10;
      }
    }
  }
  *shmaddr = shmat(*shmid, NULL, 0);
  if (*shmaddr == (void *)-1) {
    perror("shmat errorr");
    return -3;
  }
  if (NULL == *shmaddr) {
    perror("shm address == NULL");
    return -4;
  }
  memset(*shmaddr, 0, size);
  return 0;
}