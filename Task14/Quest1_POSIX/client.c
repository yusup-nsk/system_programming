#include "common_data.h"

int main() {
  int shmdes = shm_open(SHMEM_NAME, O_RDWR, 0660);
  if (-1 == shmdes) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }

  char *shmaddr =
      mmap(0, SHMEM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shmdes, 0);
  if ((void *)shmaddr == (void *)-1) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  printf("%s\n", shmaddr + 1);
  char str[MSG_LEN] = "Hello!";
  memset(shmaddr, 'C', 1);
  memcpy(shmaddr + 1, str, strlen(str) + 1);

  munmap(shmaddr, SHMEM_SIZE);
  close(shmdes);
  exit(EXIT_SUCCESS);
}