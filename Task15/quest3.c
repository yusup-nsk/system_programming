// sigwait() for the event loop
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  sigset_t set;
  int ret;
  int sig_num;

  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);

  ret = sigprocmask(SIG_BLOCK, &set, NULL);
  if (ret < 0) {
    perror("can't set blocking signal SUGUSR1!\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    printf("\nWaiting for signal SIGUSR1\n");
    sigwait(&set, &sig_num);
    if (SIGUSR1 == sig_num) {
      printf("Achieved blocked signal SIGUSR1.\n");
    }
    printf("Doing instruction inside loop\n");
  }

  exit(EXIT_SUCCESS);
}