#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t process0, process1 = -1, process2 = -2, process3 = -3, process4 = -4,
                  process5 = -5;
  process0 = getpid();
  int fin;
  pid_t pid = fork();
  process1 = pid;
  if (0 == pid) {
    process1 = getpid();
    assert(process0 == getppid());
    printf(
        "Starting process \033[1;33m #1. \033[0m My pid is \033[1;33m %d "
        "\033[0m My parent's "
        "pid is \033[1;33m %d \033[0m\n\n",
        getpid(), getppid());
  } else {
    printf(
        "Doing  process \033[1;33m #0. -main- \033[0m My pid is \033[1;33m %d "
        "\033[0m My parent's "
        "pid is \033[1;33m %d \033[0m\n\n",
        getpid(), getppid());
    assert(process1 == pid);
    pid = fork();
    process2 = pid;
    if (0 == pid) {
      printf(
          "Starting process \033[1;33m #2. \033[0m My pid is \033[1;33m %d "
          "\033[0m My parent's "
          "pid is \033[1;33m %d \033[0m\n\n",
          getpid(), getppid());
      process2 = getpid();
      assert(process0 == getppid());
    }
  }
  fin = wait(NULL);
  if (fin == process1) {
    printf("1 FINISHED process \033[1;33m #1. \033[0m\n");
  } else if (fin == process2) {
    printf("1 FINISHED process \033[1;33m #2. \033[0m\n");
  }
  fin = wait(NULL);
  if (fin == process1) {
    printf("2 FINISHED process \033[1;33m #1. \033[0m\n");
  } else if (fin == process2) {
    printf("2 FINISHED process \033[1;33m #2. \033[0m\n");
  }
  if (getpid() == process1) {
    pid = fork();
    process3 = pid;
    if (0 == pid) {
      printf(
          "Starting process \033[1;33m #3. \033[0m My pid is \033[1;33m %d "
          "\033[0m My parent's "
          "pid is \033[1;33m %d \033[0m\n\n",
          getpid(), getppid());
      process3 = getpid();
      assert(getppid() == process1);
    } else {
      pid = fork();
      process4 = pid;
      if (0 == pid) {
        printf(
            "Starting process \033[1;33m #4. \033[0m My pid is \033[1;33m %d "
            "\033[0m My parent's "
            "pid is \033[1;33m %d \033[0m\n\n",
            getpid(), getppid());
        process4 = getpid();
        assert(getppid() == process1);
      }
    }
  }

  if (getpid() == process1) {
    fin = wait(NULL);
    if (fin == process3) {
      printf("3 FINISHED process \033[1;33m #3. \033[0m\n");
    } else if (fin == process4) {
      printf("3 FINISHED process \033[1;33m #4. \033[0m\n");
    }
    fin = wait(NULL);
    if (fin == process3) {
      printf("4 FINISHED process \033[1;33m #3. \033[0m\n");
    } else if (fin == process4) {
      printf("4 FINISHED process \033[1;33m #4. \033[0m\n");
    }
  }

  if (getpid() == process2) {
    pid = fork();
    process5 = pid;
    if (0 == pid) {
      printf(
          "Starting process \033[1;33m #5. \033[0m My pid is \033[1;33m %d "
          "\033[0m My parent's "
          "pid is \033[1;33m %d \033[0m\n\n",
          getpid(), getppid());
      process5 = getpid();
      assert(getppid() == process2);
    }
  }
  if (getpid() == process2) {
    fin = wait(NULL);
    if (fin == process5) {
      printf("5 FINISHED process \033[1;33m #5. \033[0m\n");
    }
  }
  printf("process \033[1;33m %d \033[0m completed the work\n", getpid());
  exit(EXIT_SUCCESS);
}