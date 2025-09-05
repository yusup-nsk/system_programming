// block SIGINT
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>

int g_repeats = 5;
void signal_handler(int sig_num) {
  if (SIGINT == sig_num) {
    printf("\nSignal SIGINT achieved!\n");
  }
  g_repeats--;
}

int main() {
  struct sigaction handler;
  struct sigaction oldact;
  sigset_t set;
  int ret;

  sigemptyset(&set);
  sigaddset(&set, SIGINT);

  //  handler.sa_sigaction = sig_handler;
  handler.sa_handler = signal_handler;
  handler.sa_mask = set;
  // handler.sa_flags = SA_SIGINFO | SA_RESTART;

  ret = sigaction(SIGINT, &handler, &oldact);
  if (ret < 0) {
    perror("can't set signal handler!\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    if (0 == g_repeats) {
      break;
    }
    sleep(1);
  }

  ret = sigaction(SIGINT, &oldact, NULL);
  if (ret < 0) {
    perror("can't set signal handler!\n");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}