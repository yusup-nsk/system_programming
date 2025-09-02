
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#define SHMEM_SIZE 512
#define MSG_LEN 128
#define SHMEM_NAME "my_shared_memory"