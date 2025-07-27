#ifndef _GET_DIR_INFO_H_
#define _GET_DIR_INFO_H_

#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

// #include <ncurses.h>
#include <curses.h>
#include <errno.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>

#define T_LEN 13
#define LEN 128
#define MAX_FILES 256
#define MINIMUM_ROWS 10
#define MINIMUM_COLUMNS 40

typedef struct {
  char name[LEN];
  size_t size;
  char time[T_LEN];
  int type;
} Info;

enum { LEFT_WINDOW, RIGHT_WINDOW };

typedef struct {
  char directory_name[LEN];
  unsigned number_of_records;
  unsigned index_current;
  unsigned rows;
  unsigned columns;
} Frame;

int my_filter(const struct dirent *name);
int my_compar(const struct dirent **de1, const struct dirent **de2);
int get_dir_info(const char *fulldirname, Info *arr_info, unsigned *number);
int try_to_change_directory(char *fulldirname, const char *directory);
void windows_initiation(WINDOW *the_window[2], Frame the_frame[2]);
void change_directory(char *fulldirname, const char *directory);
void change_directory_up(char *fulldirname);

#endif