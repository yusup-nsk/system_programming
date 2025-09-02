#include "common_data.h"

int main() {
  int shmdes = shm_open(SHMEM_NAME, O_CREAT | O_RDWR, 0660);
  if (-1 == shmdes) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }
  int res_truncate = ftruncate(shmdes, SHMEM_SIZE);
  if (-1 == res_truncate) {
    perror("ftrancate");
    exit(EXIT_FAILURE);
  }
  char *shmaddr =
      mmap(0, SHMEM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shmdes, 0);
  if ((void *)shmaddr == (void *)-1) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  char str[MSG_LEN] = "Hi!";
  memset(shmaddr, 'S', 1);
  memcpy(shmaddr + 1, str, strlen(str) + 1);
  char ch = shmaddr[0];
  while ('S' == ch) {
    ch = shmaddr[0];
  }
  printf("%s\n", shmaddr + 1);

  munmap(shmaddr, SHMEM_SIZE);
  close(shmdes);
  shm_unlink(SHMEM_NAME);
  exit(EXIT_SUCCESS);
}