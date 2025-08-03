#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid;
  int fd[2];

  int pipe_res = pipe(fd);
  pid = fork();

  if (0 == pid) {
    close(fd[0]);
    write(fd[1], "Hi!", 4);
  } else {
    char str[30];
    close(fd[1]);
    read(fd[0], str, 29);
    printf("%s\n", str);
  }
  wait(NULL);
  return 0;
}