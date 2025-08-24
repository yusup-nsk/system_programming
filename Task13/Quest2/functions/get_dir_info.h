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

#define LEN 255
#define MINIMUM_ROWS 10
#define MINIMUM_COLUMNS 40
#define TABULATION_KEY 9
#define ESCAPE_KEY 27

extern int g_chahged_screen_size;

// typedef struct {
//   char name[LEN];
//   size_t size;
//   char time[LEN];
//   int type;
// } Info;

enum { CHAT_WINDOW, NAMES_WINDOW, INPUT_WINDOW };

typedef struct {
  unsigned number_of_records;
  unsigned index_current;
  unsigned rows;
  unsigned columns;
} Frame;

void sig_winch(int signo);
void windows_initiation(WINDOW *the_window[3], Frame the_frame[3]);

#endif