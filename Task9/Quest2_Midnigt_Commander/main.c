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

int main() {
  WINDOW *the_window[2];
  Frame the_frame[2];
  Info info_arrays[2][MAX_FILES];
  strncpy(the_frame[LEFT_WINDOW].directory_name, "/", 2);
  strncpy(the_frame[RIGHT_WINDOW].directory_name, "/", 2);
  unsigned actual_window = LEFT_WINDOW;
  freopen("/dev/nul", "w", stderr);
  initscr();
  signal(SIGWINCH, sig_winch);
  curs_set(FALSE);
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  refresh();
  init_pair(1, COLOR_YELLOW, COLOR_BLUE);
  windows_initiation(the_window, the_frame);
  for (unsigned i = 0; i < 2; ++i) {
    get_dir_info(the_frame[i].directory_name, info_arrays[i],
                 &(the_frame[i].number_of_records));
    output_the_win(the_window[i], the_frame[i], info_arrays[i],
                   i == actual_window);
  }
  int ch = ' ';
  while ((ch = getch()) != ESCAPE_KEY) {
    if (TABULATION_KEY == ch) {  // TAB
      output_the_win(the_window[actual_window], the_frame[actual_window],
                     info_arrays[actual_window], 0);
      wrefresh(the_window[actual_window]);
      actual_window = !actual_window;
    } else if (KEY_UP == ch) {
      if (the_frame[actual_window].index_current)
        the_frame[actual_window].index_current -= 1;
    } else if (KEY_DOWN == ch) {
      if (the_frame[actual_window].index_current <
          the_frame[actual_window].number_of_records - 1) {
        the_frame[actual_window].index_current += 1;
      }
    } else if (KEY_ENTER == ch || '\n' == ch) {
      if (the_frame[actual_window].index_current == 0) {
        if (strcmp(the_frame[actual_window].directory_name, "/"))
          change_directory_up(the_frame[actual_window].directory_name);
      } else {
        if (info_arrays[actual_window][the_frame[actual_window].index_current]
                .type == DT_DIR) {
          enter_to_directory(&the_frame[actual_window],
                             (info_arrays[actual_window]));
        }
      }
      get_dir_info(the_frame[actual_window].directory_name,
                   info_arrays[actual_window],
                   &(the_frame[actual_window].number_of_records));
    }
    if (g_chahged_screen_size) {
      g_chahged_screen_size = 0;
      process_change_screen_size(the_window, the_frame, info_arrays,
                                 actual_window);
      ch = ' ';
      continue;
    }
    output_the_win(the_window[actual_window], the_frame[actual_window],
                   info_arrays[actual_window], 1U);
  }  // while ch != <ESC>
  delwin(the_window[LEFT_WINDOW]);
  delwin(the_window[RIGHT_WINDOW]);
  endwin();
  exit(EXIT_SUCCESS);
}