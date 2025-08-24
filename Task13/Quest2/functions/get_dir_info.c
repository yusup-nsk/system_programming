#include "get_dir_info.h"

void sig_winch(int signo) {
  if (SIGWINCH == signo) {
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
    resizeterm(size.ws_row, size.ws_col);
    g_chahged_screen_size++;
  }
}

void windows_initiation(WINDOW *the_window[3], Frame the_frame[3]) {
  struct winsize size;
  unsigned pin_x[3], pin_y[3];
  unsigned rows[3], cols[3];
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  // mvprintw(size.ws_row - 1, 0,
  //          "<Esc>:exit   <Tab>:change window   <Enter>:open direcrory   "
  //          "Arrows<Up><Down>:scroll list");
  wattroff(stdscr, A_BOLD);
  rows[INPUT_WINDOW] = 2;
  if (size.ws_row < MINIMUM_ROWS) {
    rows[CHAT_WINDOW] = rows[NAMES_WINDOW] = MINIMUM_ROWS - 3;
  } else {
    rows[CHAT_WINDOW] = rows[NAMES_WINDOW] = size.ws_row - 3;
  }
  if (size.ws_col < MINIMUM_COLUMNS) {
    cols[CHAT_WINDOW] = 3 * MINIMUM_COLUMNS / 4;
    cols[NAMES_WINDOW] = MINIMUM_COLUMNS / 4;
    cols[INPUT_WINDOW] = MINIMUM_COLUMNS;
  } else {
    cols[CHAT_WINDOW] = cols[INPUT_WINDOW] = 3 * size.ws_col / 4;
    cols[NAMES_WINDOW] = size.ws_col / 4;
    cols[INPUT_WINDOW] = size.ws_col;
  }
  pin_x[CHAT_WINDOW] = pin_x[INPUT_WINDOW] = 0;
  pin_x[NAMES_WINDOW] = pin_x[CHAT_WINDOW] + cols[CHAT_WINDOW];
  pin_y[CHAT_WINDOW] = pin_y[NAMES_WINDOW] = 0;
  pin_y[INPUT_WINDOW] = pin_y[CHAT_WINDOW] + rows[CHAT_WINDOW];
  for (int i = 0; i < 3; ++i) {
    the_frame[i].index_current = 0;
    the_frame[i].rows = rows[i];
    the_frame[i].columns = cols[i];
    the_window[i] = newwin(rows[i], cols[i], pin_y[i], pin_x[i]);
    wbkgd(the_window[i], COLOR_PAIR(i + 1));
    wrefresh(the_window[i]);
  }
}