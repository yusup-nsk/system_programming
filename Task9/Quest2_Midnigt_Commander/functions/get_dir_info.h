#ifndef _GET_DIR_INFO_H_
#define _GET_DIR_INFO_H_

#include <curses.h>
#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <termios.h>
#include <time.h>

#define LEN 128  // максимальная длина названия файла или директории
#define MAX_FILES \
  512  // максимальное количество записей в директории, которые способно
       // показать программа
#define MINIMUM_ROWS 10
#define MINIMUM_COLUMNS 40
#define TABULATION_KEY 9
#define ESCAPE_KEY 27

extern int g_chahged_screen_size;

typedef struct {
  char name[LEN];
  size_t size;
  char time[LEN];
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

void sig_winch(int signo);
int my_filter(const struct dirent *name);
int my_compar(const struct dirent **de1, const struct dirent **de2);
int try_to_change_directory(char *fulldirname, const char *directory);
void windows_initiation(WINDOW *the_window[2], Frame the_frame[2]);
void change_directory(char *fulldirname, const char *directory);
void change_directory_up(char *fulldirname);
int get_dir_info(const char *fulldirname, Info *arr_info, unsigned *number);

#endif