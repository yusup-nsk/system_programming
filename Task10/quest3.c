#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define LEN 12
#define MAX_LINE 512
#define NUM 20
#define SZ 2

int input_arr(char ***arr, unsigned *count);
void output_arr(char **arr, unsigned count);
void free_arr(char **arr, unsigned count);

int main() {
  unsigned count;
  char **arr = NULL;
  int repeat=1;
  while(repeat){
  int res = input_arr(&arr, &count);
  if (-1 == res) {
    printf("\nInput failed\n\n");
  }
 if (strncmp(arr[0], "exit", 5)){
  pid_t pid = fork();
  if (0==pid){
    execvp(arr[0], arr);
  }
     wait(NULL);  
     kill(pid, SIGTERM);
 } else repeat=0;

  free_arr(arr, count);
} // while repeat

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
  if (0 == line[0]) {
    *count = 0;
    *arr = NULL;
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