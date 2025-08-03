#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#define LENGTH 10

int main() {
  char pipename[] = "pipe_server_to_client";
  char str[LENGTH];

  int fdes = open(pipename, O_RDONLY);
  if (-1 == fdes) {
    perror("Client failed to open pipe");
    exit(EXIT_FAILURE);
  }
  read(fdes, str, LENGTH);
  str[LENGTH - 1] = 0;
  printf("%s\n", str);
  close(fdes);

  exit(EXIT_SUCCESS);
}