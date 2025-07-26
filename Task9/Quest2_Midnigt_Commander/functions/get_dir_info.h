#ifndef _GET_DIR_INFO_H_
#define _GET_DIR_INFO_H_

#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define T_LEN 13
#define LEN 128
#define MAX_FILES 256

typedef struct {
  char name[LEN];
  size_t size;
  char time[T_LEN];
  int type;
} Info;

int my_filter(const struct dirent *name);
int my_compar(const struct dirent **de1, const struct dirent **de2);
int get_dir_info(const char *fulldirname, Info *arr_info, unsigned *number);

#endif