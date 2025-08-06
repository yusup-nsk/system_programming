#include "functions.h"

int main() {
  unsigned count;
  char **arr = NULL;
  int repeat = 1;
  while (repeat) {
    sleep(2);
    int res = input_arr(&arr, &count);
    if (-1 == res) {
      printf("\nInput failed\n\n");
      continue;
    } else if (0 == res) {
      continue;
    }
    if (strncmp(arr[0], "exit", 5)) {
      // pid_t pid = fork();
      int pid = 0;
      if (0 == pid) {
        char **token = NULL;
        unsigned index_in_arr = 0;
        while (index_in_arr < count) {
          int cnt_token = get_token(arr, &token, &index_in_arr);
          printf("cnt_token = %d; pointer token = %p\n", cnt_token,
                 (void *)token);
          if (-1 == cnt_token) {
          } else if (0 == cnt_token) {
            token = arr;  // + index_in_arr;
            // execve(arr[0], arr);
          } else {
            for (int i = 0; i < cnt_token; ++i) {
              if (token[i])
                printf("token[%d]:<%s>\n", i, token[i]);
              else
                printf("token[%d]:<%s>\n", i, "NUL");
            }

            //  execvp(token[0], token);
          }

          free_arr(token, cnt_token);
        }
      }
      // wait(NULL);
      // kill(pid, SIGTERM);
    } else
      repeat = 0;

    free_arr(arr, count);
  }  // while repeat

  return 0;
}

