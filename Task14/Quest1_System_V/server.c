#include "common_data.h"

int main() {
  int shmid;
  char *shmaddr;
  char filename[2 * MSG_LEN];
  char msg[MSG_LEN];
  strncpy(msg, "Hi!", MSG_LEN - 1);
  sprintf(filename, "%s/%s", getenv("PWD"), FILE_TO_KEY);

  key_t key = ftok(filename, NUMBER_TO_KEY);
  if (-1 == key) {
    perror("creating key");
    exit(EXIT_FAILURE);
  }

  if ((shmid = shmget(key, SHMEM_SIZE, IPC_CREAT | 0660)) == -1) {
    perror("getting message");
    exit(EXIT_FAILURE);
  }

  shmaddr = shmat(shmid, NULL, 0);
  if ((void *)shmaddr == (void *)-1) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  sprintf(shmaddr, "%c%s", 'S', msg);

  while ('C' != shmaddr[0]) {
  }

  printf("%s\n", shmaddr + 1);

  shmctl(shmid, IPC_RMID, NULL);

  exit(EXIT_SUCCESS);
}