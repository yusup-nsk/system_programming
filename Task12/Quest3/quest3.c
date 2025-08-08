#include "functions.h"
unsigned g_i =0;

int main() {
  unsigned count;
  char **arr = NULL;
  int repeat = 1;
  while (repeat) {
    sleep(2);
    int res = input_arr(&arr, &count);
    if (-1 == res) {
      // printf("\nInput failed\n\n");
      continue;
    } else if (-1 == res) {
      printf("Empty input.\n");
      continue;
    } else if (0>res) {
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
          // int cnt_token = get_token(arr, &token, &index_in_arr);
          // g_i = index_in_arr;
          printf("cnt_token = %d; pointer token = %p\n", cnt_token,
                 (void *)token);
          if (-1 == cnt_token) {        // TODO
          } else if (0 == cnt_token) {  
            printf("\nTrunc when cnt_token = %d\n\n", cnt_token);
          } else {
            for (int i = 0; i < cnt_token ; ++i) {
              if (token[i])
                printf("token[%d]:<%s>\n", i, token[i]);
              else
                printf("token[%d]:<%s>\n", i, "NUL");
            }

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
            // dup2(1,pipefd[0]);
            if (0 == cpid) {
              close(pipefd[0]);
              dup2(pipefd[1], 1);
              execvp(token[0], token);
            } else {

              cnt_token = get_token(arr, &token, &g_i);
              dup2(pipefd[0],0);
              // dup2(pipefd[1], 1);
              close(pipefd[1]);
              execvp(token[0], token);
              // close(pipefd[1]);


              wait(NULL);
            }
          
            // execvp(token[0], token);
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
