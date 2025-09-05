// make disposition for SIGUSR1
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>

int g_repeats = 3;
void sig_handler(int sig_num, siginfo_t *info, void *context) {
  if (SIGUSR1 == sig_num) {
    printf("Signal SIGUSR1 achieved\n");
  } else {
    printf("Signal  #%d achieved\n \n", sig_num);
  }
  if (g_repeats == 1) {
    printf("\nFirst argument is signal number\n");
    printf("Second arguments of function sa_sighandler is 'siginfo_t info' "
           "wich is a structure "
           "with fields:\n");
    printf("  si_signo = %d\n", info->si_signo);
    printf("  si_errno = %d\n", info->si_errno);
    printf("  si_code = %d\n", info->si_code);
    printf("  si_pid = %d\n", info->si_pid);
    printf("  si_uid = %d\n", info->si_uid);
    printf("  si_status = %d\n  .....\n", info->si_status);
    printf("  si_int = %d\n", info->si_int);
    printf("  si_overrun = %d\n", info->si_overrun);
    printf("  si_addr = %p\n", info->si_addr);
    printf("  si_band = %ld\n", info->si_band);
    printf("  si_fd = %d\n", info->si_fd);
    printf("  si_addr_lsb = %d\n", info->si_addr_lsb);
    printf("  si_pkey = %d\n", info->si_pkey);
    printf("  si_call_addr = %p\n", info->si_call_addr);
    printf("  si_syscall = %d\n", info->si_syscall);
    printf("  si_arch = %u\n", info->si_arch);

    ucontext_t ucontext = *(ucontext_t *)context;
    printf(
        "\nThird argument is 'ucontext_t * ucontext' wich is a struct of:\n");
    printf("  pointer uc_link = %p\n", (void *)ucontext.uc_link);
    printf("  stack_t uc_stack is a struct of\n");
    printf("    void *ss_p = %p\n", ucontext.uc_stack.ss_sp);
    printf("    size_t ss_size = %lu\n", ucontext.uc_stack.ss_size);
    printf("    int ss_flags = %x\n", ucontext.uc_stack.ss_flags);
    printf("    ....\n");
  }
  g_repeats--;
}

int main() {
  struct sigaction handler;
  sigset_t set;
  int ret;

  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);

  handler.sa_sigaction = sig_handler;
  handler.sa_mask = set;
  handler.sa_flags = SA_SIGINFO | SA_RESTART;

  ret = sigaction(SIGUSR1, &handler, NULL);
  if (ret < 0) {
    perror("can't set signal handler!\n");
    exit(EXIT_FAILURE);
  }

  while (g_repeats) {
    sleep(1);
  }

  exit(EXIT_SUCCESS);
}