#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#define LENGTH 128

int main() {
  char pipename[] = "pipe_server_to_client";
  char str[LENGTH] = {0};

  int fdes = open(pipename, O_RDONLY);
  if (-1 == fdes) {
    sprintf(str, "Client failed to open pipe \"%s\"", pipename);
    perror(str);
    exit(EXIT_FAILURE);
  }
  int read_result = read(fdes, str, LENGTH);
  if (read_result == -1) {
    sprintf(str, "Client failed to read from pipe \"%s\"", pipename);
    perror(str);
    exit(EXIT_FAILURE);
  } else if (read_result == 0) {
    printf("Empty pipe\n");
  }
  str[LENGTH - 1] = 0;
  printf("%s\n", str);
  close(fdes);

  exit(EXIT_SUCCESS);
}