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
  refresh();
  init_pair(1, COLOR_BLUE, COLOR_GREEN);
  init_pair(2, COLOR_YELLOW, COLOR_BLUE);
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  int lrows, rrows, lcols, rcols;
  int lpin_x, lpin_y, rpin_x, rpin_y;
  lrows = rrows = size.ws_row - 2;
  lcols = rcols = size.ws_col / 2 - 4;
  lpin_x = 1;
  lpin_y = 1;
  rpin_x = lcols + lpin_x + 2;
  rpin_y = 1;
  the_window[LEFT_WINDOW] = newwin(lrows, lcols, lpin_y, lpin_x);
  the_window[RIGHT_WINDOW] = newwin(rrows, rcols, rpin_y, rpin_x);
  get_dir_info(dir_name[LEFT_WINDOW], two_info_arrays[LEFT_WINDOW],
               &num_records[LEFT_WINDOW]);
  get_dir_info(dir_name[RIGHT_WINDOW], two_info_arrays[RIGHT_WINDOW],
               &num_records[RIGHT_WINDOW]);

  char ch;
  while (ch != 'q' && ch != 'Q') {
    if (9 == ch) {  // TAB
      actual_window = !actual_window;
    }
    if (g_chahged_screen_size) {
      g_chahged_screen_size = 0;
      delwin(the_window[LEFT_WINDOW]);
      delwin(the_window[RIGHT_WINDOW]);
      ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
      lrows = rrows = size.ws_row - 2;
      lcols = rcols = size.ws_col / 2 - 4;
      lpin_x = 1;
      lpin_y = 1;
      rpin_x = lcols + lpin_x + 2;
      rpin_y = 1;
      the_window[LEFT_WINDOW] = newwin(lrows, lcols, lpin_y, lpin_x);
      the_window[RIGHT_WINDOW] = newwin(rrows, rcols, rpin_y, rpin_x);
    }

    wbkgd(the_window[actual_window], COLOR_PAIR(1));
    box(the_window[actual_window], '|', '-');
    wattron(the_window[actual_window], A_BOLD);
    keypad(the_window[actual_window], TRUE);
    wprintw(the_window[actual_window], "Enter password...\n");
    output_to_window(the_window[actual_window], two_info_arrays[actual_window],
                     num_records[actual_window], lrows - 1, lcols - 2,
                     current_index[actual_window]);
    wrefresh(the_window[actual_window]);
    cbreak();
    ch = getch();
  }  // while ch != q Q
  delwin(the_window[LEFT_WINDOW]);
  delwin(the_window[RIGHT_WINDOW]);
  curs_set(FALSE);
  refresh();
  // getch();
  endwin();
  exit(EXIT_SUCCESS);
}