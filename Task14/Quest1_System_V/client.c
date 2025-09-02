#include "common_data.h"

int main() {
  int shm_id;
  char *shmaddr;
  char filename[2 * MSG_LEN];
  char message[MSG_LEN] = "Hello!";
  sprintf(filename, "%s/%s", getenv("PWD"), FILE_TO_KEY);

  key_t key = ftok(filename, NUMBER_TO_KEY);
  if (-1 == key) {
    perror("function ftok");
    exit(EXIT_FAILURE);
  }

  shm_id = shmget(key, SHMEM_SIZE, 0);
  if (-1 == shm_id) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  shmaddr = shmat(shm_id, NULL, 0);
  if ((void *)shmaddr == (void *)-1) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  while ('S' != shmaddr[0]) {
  }
  printf("%s\n", shmaddr + 1);
  memset(shmaddr, 0, SHMEM_SIZE);
  sprintf(shmaddr, "%c%s", 'C', message);

  exit(EXIT_SUCCESS);
}
