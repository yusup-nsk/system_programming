#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define LEN 128
#define MAX_LINE 512
#define SZ 32

void output_arr(char **arr, unsigned count);
int input_arr(char ***arr, unsigned *count);
void free_arr(char **arr, unsigned count);
int get_token(char **arr, char ***token, unsigned *index_in_arr) {
  if (!(arr)) {
    return -1;
  }

  int count = 0;
  // output_arr(arr, 2);
  char **ptr = arr + *index_in_arr;
  for (; ptr && *ptr; ptr++, count++) {
    if (*ptr)
      printf("*ptr=%s\n", *ptr);
    else
      printf("*ptr=%s\n", "Null");
    if (strncmp(*ptr, "|", 2) == 0) {
      break;
    }
  }
  if (NULL == *ptr || 0 == count) {
    return 0;
  }

  count++;
  *token = (char **)malloc((count) * sizeof(char *));
  for (int i = 0; i < count - 1; i++) {
    unsigned size = strlen(arr[*index_in_arr + i]) + 1;
    (*token)[i] = (char *)malloc(size * sizeof(char));
    memcpy((*token)[i], arr[*index_in_arr + i], size);
  }
  (*token)[count - 1] = NULL;
  output_arr(*token, count);
  *index_in_arr += count;
  return count;
}

int main() {
  unsigned count;
  char **arr = NULL;
  int repeat = 1;
  while (repeat) {
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
        int cnt_token = get_token(arr, &token, &index_in_arr);
        printf("cnt_token = %d; pointer token = %p\n", cnt_token,
               (void *)token);
        if (-1 == cnt_token) {
        } else if (0 == cnt_token) {
          token = arr;
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
      // wait(NULL);
      // kill(pid, SIGTERM);
    } else
      repeat = 0;

    free_arr(arr, count);
  }  // while repeat

  return 0;
}

int input_arr(char ***arr, unsigned *count) {
  if (!(arr && count)) {
    return -1;
  }
  *count = 0;
  printf("Input command: ");
  char line[MAX_LINE];
  fgets(line, MAX_LINE, stdin);
  if (0 == line[0] || '\n' == line[0]) {
    *count = 0;
    // *arr = NULL;
    return 0;
  } else
    line[MAX_LINE - 1] = 0;
  unsigned sz = SZ;
  *arr = (char **)malloc(sz * sizeof(char *));
  if (NULL == *arr) {
    printf("Failed malloc for array\n");
    return -1;
  }
  for (char *s = strtok(line, " \t\n"); s; s = strtok(NULL, " \t\n")) {
    if (sz < *count + 3) {
      sz *= 2;
      char **tmp = (char **)realloc(*arr, sz * sizeof(char *));
      assert(tmp);
      *arr = tmp;
    }
    (*arr)[*count] = (char *)malloc(LEN);
    char format[LEN];
    sprintf(format, "%%%ds", LEN - 1);
    sscanf(s, format, (*arr)[*count]);
    (*arr)[*count][LEN - 1] = 0;
    *count += 1;
  }
  (*arr)[*count] = NULL;
  *count += 1;
  return sz;
}

void free_arr(char **arr, unsigned count) {
  if (arr) {
    for (unsigned i = 0; i < count; ++i) {
      if (arr[i]) {
        free(arr[i]);
      }
    }
    free(arr);
  }
}

void output_arr(char **arr, unsigned count) {
  if (arr) {
    for (unsigned i = 0; i < count; ++i) {
      if (arr[i]) {
        printf("%u:  %s\n", i, arr[i]);
      } else
        printf("%u:  %s\n", i, "NULL");
    }
  }
}