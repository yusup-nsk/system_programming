#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define LENGTH 30

int main() {
  pid_t pid;
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  pid = fork();
  if (-1 == pid) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  if (0 == pid) {
    close(pipefd[0]);
    write(pipefd[1], "Hi!", 4);
  } else {
    char str[LENGTH];
    close(pipefd[1]);
    read(pipefd[0], str, 29);
    printf("\"%s\"\n", str);
  }
  wait(NULL);
  exit(EXIT_SUCCESS);
}