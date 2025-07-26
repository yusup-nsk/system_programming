#include <curses.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "functions/get_dir_info.h"
#include "functions/output_to_window.h"

int g_chahged_screen_size = 0;
void sig_winch(int signo) {
  if (SIGWINCH == signo) {
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
    resizeterm(size.ws_row, size.ws_col);
    g_chahged_screen_size++;
  }
}

enum { LEFT_WINDOW, RIGHT_WINDOW };

int main() {
  WINDOW *the_window[2];
  Info two_info_arrays[2][MAX_FILES];
  char dir_name[2][LEN] = {"/", "/"};
  unsigned num_records[2];
  int actual_window = LEFT_WINDOW;
  unsigned current_index[2] = {0, 0};

  initscr();
  signal(SIGWINCH, sig_winch);
  curs_set(TRUE);
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  refresh();
  init_pair(1, COLOR_BLUE, COLOR_GREEN);
  init_pair(2, COLOR_YELLOW, COLOR_BLUE);
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  unsigned rows[2], cols[2], pin_x[2], pin_y[2];
  rows[LEFT_WINDOW] = rows[RIGHT_WINDOW] = size.ws_row - 2;
  cols[LEFT_WINDOW] = cols[RIGHT_WINDOW] = size.ws_col / 2 - 4;
  pin_x[LEFT_WINDOW] = 1;
  pin_x[RIGHT_WINDOW] = cols[LEFT_WINDOW] + pin_x[LEFT_WINDOW] + 2;
  pin_y[LEFT_WINDOW] = pin_y[RIGHT_WINDOW] = 1;
  for (int i = 0; i < 2; ++i) {
    the_window[i] = newwin(rows[i], cols[i], pin_y[i], pin_x[i]);
  }
  get_dir_info(dir_name[LEFT_WINDOW], two_info_arrays[LEFT_WINDOW],
               &num_records[LEFT_WINDOW]);
  get_dir_info(dir_name[RIGHT_WINDOW], two_info_arrays[RIGHT_WINDOW],
               &num_records[RIGHT_WINDOW]);

  int ch;
  while (ch != 'q' && ch != 'Q') {
    if (9 == ch) {  // TAB
      actual_window = !actual_window;
    } else if (KEY_UP == ch) {
      if (current_index[actual_window]) current_index[actual_window] -= 1;
    } else if (KEY_DOWN == ch) {
      if (current_index[actual_window] < rows[actual_window] - 1)
        current_index[actual_window] += 1;
    }
    if (g_chahged_screen_size) {
      g_chahged_screen_size = 0;
      delwin(the_window[LEFT_WINDOW]);
      delwin(the_window[RIGHT_WINDOW]);
      ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
      rows[LEFT_WINDOW] = rows[RIGHT_WINDOW] = size.ws_row - 2;
      cols[LEFT_WINDOW] = cols[RIGHT_WINDOW] = size.ws_col / 2 - 2;
      pin_x[LEFT_WINDOW] = 1;
      pin_x[RIGHT_WINDOW] = cols[LEFT_WINDOW] + pin_x[LEFT_WINDOW] + 2;
      pin_y[LEFT_WINDOW] = pin_y[RIGHT_WINDOW] = 1;
      for (int i = 0; i < 2; ++i) {
        the_window[i] = newwin(rows[i], cols[i], pin_y[i], pin_x[i]);
      }
    }

    wbkgd(the_window[actual_window], COLOR_PAIR(2));
    wprintw(the_window[actual_window], ".../%sn", dir_name[actual_window]);
    wmove(the_window[actual_window], 3, pin_x[LEFT_WINDOW] + 1);
    output_to_window(the_window[actual_window], two_info_arrays[actual_window],
                     num_records[actual_window], rows[LEFT_WINDOW] - 5,
                     cols[LEFT_WINDOW] - 4, current_index[actual_window]);
    wrefresh(the_window[actual_window]);
    cbreak();
    ch = getch();
  }  // while ch != q Q
  delwin(the_window[LEFT_WINDOW]);
  delwin(the_window[RIGHT_WINDOW]);
  curs_set(FALSE);
  refresh();
  endwin();
  exit(EXIT_SUCCESS);
}