#include "windows_procedures.h"

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

void output_chat_and_names_windows(WINDOW *the_window[3],
                                   Data_of_client clientdata, Frame frame[3],
                                   unsigned *start_chat) {
  unsigned y;
  int doitagain = 1;
  while (doitagain) {
    wmove(the_window[CHAT_WINDOW], 0, 0);
    for (unsigned r = 0; r < frame[CHAT_WINDOW].rows; r++) {
      wprintw(the_window[CHAT_WINDOW], "\n");
    }
    wmove(the_window[CHAT_WINDOW], 0, 0);
    unsigned chatindex = *start_chat;
    for (; chatindex < clientdata.size_chat; chatindex++) {
      wprintw(the_window[CHAT_WINDOW], "%s", clientdata.chat[chatindex]);
      y = getcury(the_window[CHAT_WINDOW]);
      if (y >= frame[CHAT_WINDOW].rows - 1) {
        *start_chat += 1;
        break;
      }
      wprintw(the_window[CHAT_WINDOW], "\n");
    }
    if (chatindex <= clientdata.size_chat) {
      doitagain = 0;
    }
  }
  doitagain = 1;
  unsigned start_names_index = 0;
  while (doitagain) {
    wmove(the_window[NAMES_WINDOW], 0, 0);
    for (unsigned r = 0; r < frame[NAMES_WINDOW].rows; r++) {
      wprintw(the_window[NAMES_WINDOW], "\n");
    }
    wmove(the_window[NAMES_WINDOW], 0, 0);
    unsigned i = start_names_index;
    for (; i < clientdata.size_names; ++i) {
      wprintw(the_window[NAMES_WINDOW], " %s", clientdata.other_names[i]);
      y = getcury(the_window[NAMES_WINDOW]);
      if (y >= frame[NAMES_WINDOW].rows-1)  {
        start_names_index++;
        break;
      }
      wprintw(the_window[NAMES_WINDOW], "\n");
    }
    if (i <= clientdata.size_names) {
      doitagain = 0;
    }
  }
  wrefresh(the_window[0]);
  wrefresh(the_window[1]);
}

void process_change_screen_size(WINDOW **window, Frame *frame,
                                Data_of_client clientdata,
                                unsigned *start_chat) {
  for (unsigned i = 0; i < 3; ++i) {
    delwin(window[i]);
  }
  erase();
  refresh();
  windows_initiation(window, frame);
  for (unsigned i = 0; i < 3; ++i) {
    werase(window[i]);
  }
  output_chat_and_names_windows(window, clientdata, frame, start_chat);
  wmove(window[INPUT_WINDOW], 0, 0);
  wprintw(window[INPUT_WINDOW], "\n\n");
  wmove(window[INPUT_WINDOW], 0, 0);
  wprintw(window[INPUT_WINDOW], "[%s]:\n", clientdata.name);
  wrefresh(window[INPUT_WINDOW]);
  refresh();
}
