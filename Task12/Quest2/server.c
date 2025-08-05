#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  char pipename[] = "pipe_server_to_client";

  int res = mkfifo(pipename, 0600);
  if (-1 == res && errno != EEXIST) {
    perror("Server failed to make the pipe");
    exit(EXIT_FAILURE);
  }
  int fdes = open(pipename, O_WRONLY);
  if (-1 == res && errno != EEXIST) {
    perror("Server could not open the pipe");
    exit(EXIT_FAILURE);
  }
  write(fdes, "Hi!", 4);

  exit(EXIT_SUCCESS);
}