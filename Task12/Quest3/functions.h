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


int get_indexes_of_vertical_bars(char **arr, unsigned arr_size,  unsigned indexes_of_vertical_bars[MAX_PIPES], unsigned *n_vertical_bars);

int make_string_of_commands(char **arr, unsigned arr_size, char ***command_and_arguments, unsigned *cmd_and_arg_size, unsigned *indexes_of_vertical_bars, unsigned n_vertical_bars, unsigned i_of_command) ;


#endif