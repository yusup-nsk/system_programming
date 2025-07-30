#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int status = 2025;
  pid_t pid = fork();

  if (0 == pid) {
    printf(
        "This is daughter process. My pid is \033[1;33m%d\033[0m. My "
        "parent's pid is \033[1;33m%d\033[0m.\n\n",
        getpid(), getppid());
  } else {
    printf(
        "This is parent process. My pid is \033[1;33m%d\033[0m. My parent's "
        "pid is \033[1;33m%d\033[0m.\n\n",
        getpid(), getppid());
  }

  wait(&status);

  if (0 == pid) {
    printf("Daughter process is running...\n");
    printf("Daughter process is finished\n\n");
  } else {
    printf(
        "Parent: Status of finising daughter process is \033[1;33m%d\033[0m.\n",
        status);
  }
  exit(EXIT_SUCCESS);
}