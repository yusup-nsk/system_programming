// sigwait()
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>

int g_repeats = 3;
void signal_handler(int sig_num) {
  if (SIGUSR2 == sig_num) {
    printf("\nSignal SIGUSR2 achieved!\n\n");
  }
  g_repeats--;
}

int main() {
  struct sigaction handler;
  struct sigaction oldact;
  sigset_t set, set2, set3;
  int ret;
  int sig_num;

  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);

  //  handler.sa_sigaction = sig_handler;
  handler.sa_handler = signal_handler;
  handler.sa_mask = set;
  // handler.sa_flags = SA_SIGINFO | SA_RESTART;

  // ret = sigaction(SIGUSR1, &handler, &oldact);
  ret = sigprocmask(SIG_BLOCK, &set, NULL);
  if (ret < 0) {
    perror("can't set blocking signal SUGUSR1!\n");
    exit(EXIT_FAILURE);
  }
  ret = sigaction(SIGUSR2, &handler, &oldact);
  for (int i = 0; i <= 64; i++) {
    if (sigismember(&set, i)) {
      printf("%d is member\n", i);
    }
  }
  sigpending(&set3);
  for (int i = 0; i <= 64; i++) {
    if (sigismember(&set, i)) {
      printf("%d is member of pending set\n", i);
    }
  }
  while (1) {
    // sleep(3);
    if (0 == g_repeats) {
      break;
    } 
    printf("\nWaiting signal SIGUSR1\n");
    // sigwait(&set, &sig_num);
    // if (SIGUSR1 == sig_num) {
    //   printf("Achieved blocked signal SIGUSR1.\n");
    // }
    printf("Doing instruction inside loop\ng_repeats = %d\n", g_repeats);
  }

  sigpending(&set3);
  for (int i = 0; i <= 64; i++) {
    if (sigismember(&set, i)) {
      printf("%d is member of pending set\n", i);
    }
  }
  exit(EXIT_SUCCESS);
}