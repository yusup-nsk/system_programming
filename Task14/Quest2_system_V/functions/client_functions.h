#ifndef _CLIENT_FUNCTIONS_H_
#define _CLIENT_FUNCTIONS_H_

#include <signal.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <termios.h>

#include "common_data.h"

#define LEN 255
#define MINIMUM_ROWS 10
#define MINIMUM_COLUMNS 40
#define MAX_TRY_CONNECT 20
#define MSEC_BETWEEN_TRY_CONNECT 500000

extern int g_chahged_screen_size;

enum { CHAT_WINDOW, NAMES_WINDOW, INPUT_WINDOW };

typedef struct {
  unsigned number_of_records;
  unsigned index_current;
  unsigned rows;
  unsigned columns;
} Frame;

typedef struct {
  unsigned id;
  char name[NAME_LEN];
  char chat[MAX_NUM_CHAT][CHAT_MSG_LEN];
  unsigned size_chat;
  char other_names[MAX_CLIENTS + 1][NAME_LEN];
  unsigned size_names;
} Data_of_client;

void sig_winch(int signo);
void windows_initiation(WINDOW *the_window[3], Frame the_frame[3]);
void input_name(char *name);
int get_shared_memory(int *shmid, void **shmaddr, unsigned size,
                      char *file_to_key, unsigned number_to_key);
void output_chat_and_names_windows(WINDOW *the_window[3],
                                   Data_of_client clientdata, Frame frame[3],
                                   unsigned *start_chat);

void send_message_to_chat(ChatData *shmaddr_chat, const char *name,
                          const char *message);
int get_mutex_semaphore(int *mutex_sem, const char *file, int proj_id);

void process_change_screen_size(WINDOW **window, Frame *frame);

#endif
