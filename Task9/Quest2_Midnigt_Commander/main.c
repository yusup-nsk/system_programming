#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
// int g_chahged_window = 0;
void sig_winch(int signo) {
  if (SIGWINCH == signo) {
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
    resizeterm(size.ws_row, size.ws_col);
    // g_chahged_window++;
  }
}

int main() {
  WINDOW *wnd;
  WINDOW *left_w, *right_w;
  // int changes_of_screen = g_chahged_window;

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
  //   lrows = rrows = size.ws_row - 2;
  //   lcols = rcols = size.ws_col / 2 - 4;
  //   lpin_x = 1;
  //   lpin_y = 1;
  //   rpin_x = lcols + lpin_x + 2;
  //   rpin_y = 1;
  //   left_w = newwin(lrows, lcols, lpin_y, lpin_x);
  //   right_w = newwin(rrows, rcols, rpin_y, rpin_x);

  char ch;
  while (ch != 'q' && ch != 'Q') {
    ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
    lrows = rrows = size.ws_row - 2;
    lcols = rcols = size.ws_col / 2 - 4;
    lpin_x = 1;
    lpin_y = 1;
    rpin_x = lcols + lpin_x + 2;
    rpin_y = 1;
    left_w = newwin(lrows, lcols, lpin_y, lpin_x);
    right_w = newwin(rrows, rcols, rpin_y, rpin_x);
    wnd = left_w;
    wbkgd(wnd, COLOR_PAIR(1));
    box(wnd, '|', '_');
    wattron(wnd, A_BOLD);
    keypad(wnd, TRUE);
    wprintw(wnd, "Enter password...\n");
    wattron(wnd, A_BLINK);
    wprintw(wnd, "ACCESS DENIED!");
    wrefresh(wnd);

    wnd = right_w;
    wbkgd(wnd, COLOR_PAIR(2));
    wattron(wnd, A_BOLD);
    keypad(wnd, TRUE);
    wprintw(wnd, "Enter password...\n");
    wattron(wnd, A_BLINK);
    wprintw(wnd, "ACCESS GRANTED!");
    wrefresh(wnd);
    cbreak();
    ch = getch();
  }
  delwin(left_w);
  delwin(right_w);
  curs_set(FALSE);
  move(8, 4);
  printw("Press any key to continue...");
  refresh();
  // getch();
  endwin();
  exit(EXIT_SUCCESS);
}