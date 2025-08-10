#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

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
#define MAX_PIPES 32

// void output_arr(char **arr, unsigned count);
int input_arr(char ***arr, unsigned *count);
void free_arr(char **arr, unsigned count);
int input_string(char *line, unsigned *len);
int get_token(char **arr, char ***token, unsigned *index_in_arr);

#endif