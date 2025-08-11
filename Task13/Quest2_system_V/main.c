#include "functions/output_to_window.h"
#include "functions/windows_procedures.h"

int g_chahged_screen_size = 0;

int main() {
  WINDOW *the_window[3];
  Frame the_frame[3];
  unsigned actual_window = 0;
  FILE *errors_log = freopen("errors.log", "w", stderr);
  initscr();
  signal(SIGWINCH, sig_winch);
  curs_set(FALSE);
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  refresh();
  init_pair(1, COLOR_YELLOW, COLOR_BLUE);
  init_pair(2, COLOR_RED, COLOR_GREEN);
  init_pair(3, COLOR_WHITE, COLOR_BLACK);

  windows_initiation(the_window, the_frame);

  int ch;
  while ((ch = getch()) != ESCAPE_KEY) {
    if (TABULATION_KEY == ch) {
    } else if (KEY_UP == ch) {
    } else if (KEY_DOWN == ch) {
    } else if (KEY_ENTER == ch || '\n' == ch) {
    }
    if (g_chahged_screen_size) {
      g_chahged_screen_size = 0;
      process_change_screen_size(the_window, the_frame, actual_window);
      ch = ' ';
      continue;
    }
    draw_horizontal_line(the_window[0], 144, '2');
    draw_horizontal_line(the_window[1], 144, 'r');
    draw_horizontal_line(the_window[2], 154, '=');
    wrefresh(the_window[0]);
    wrefresh(the_window[1]);
    wrefresh(the_window[2]);
    refresh();
    output_the_win(the_window[actual_window], the_frame[actual_window], 1U);
  }  // while ch != <ESC>
  delwin(the_window[0]);
  delwin(the_window[1]);
  delwin(the_window[2]);
  endwin();
  if (errors_log) fclose(errors_log);
  exit(EXIT_SUCCESS);
}
