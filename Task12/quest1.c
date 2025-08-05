#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#define LENGTH 30

int main() {
  pid_t pid;
  int fd[2];
  pipe(fd);
  pid = fork();

  if (0 == pid) {
    close(fd[0]);
    write(fd[1], "Hi!", 4);
  } else {
    char str[LENGTH];
    close(fd[1]);
    read(fd[0], str, 29);
    printf("%s\n", str);
  }
  
  wait(NULL);
  return 0;
}