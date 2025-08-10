#include "functions.h"
unsigned g_i = 0;

int main() {
  unsigned count;
  char **arr = NULL;
  int repeat = 1;
  while (repeat) {
    int res = input_arr(&arr, &count);
    if (-1 == res) {
      continue;
    } else if (-1 == res) {
      printf("Empty input.\n");
      continue;
    } else if (0 > res) {
      printf("Syntax error\n");
      continue;
    }
    if (strncmp(arr[0], "exit", 5)) {
      pid_t pid = fork();
      if (0 == pid) {
        char **token = NULL;
        unsigned index_in_arr = 0;
        while (index_in_arr < count) {
          int cnt_token = get_token(arr, &token, &g_i);
          if (cnt_token > 0) {
            int pipefd[2];
            pid_t cpid;
            if (pipe(pipefd) == -1) {
              perror("pipe");
              exit(EXIT_FAILURE);
            }
            cpid = fork();
            if (-1 == cpid) {
              perror("fork");
              exit(EXIT_FAILURE);
            }
            if (0 == cpid) {
              close(pipefd[0]);
              dup2(pipefd[1], 1);
              execvp(token[0], token);
            } else {
              cnt_token = get_token(arr, &token, &g_i);
              dup2(pipefd[0], 0);
              close(pipefd[1]);
              execvp(token[0], token);
              wait(NULL);
            }
          }
          free_arr(token, cnt_token);
        }
      } else  // pid > 0
      {
        wait(NULL);
        kill(pid, SIGTERM);
      }
    } else {
      repeat = 0;
    }
    free_arr(arr, count);
  }  // while repeat

  return 0;
}
