// TODO при превышениии максимум файлов - переписать массив - файлы от 256 до
// 512

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
  int actual_window = LEFT_WINDOW;
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
  init_pair(2, COLOR_RED, COLOR_BLACK);

  windows_initiation(the_window, the_frame);
  for (int i = 0; i < 2; ++i) {
    get_dir_info(the_frame[i].directory_name, info_arrays[i],
                 &(the_frame[i].number_of_records));
  }
  chtype ch = ' ';
  while (27 != ch) {
    if (9 == ch) {  // TAB
      actual_window = !actual_window;
    } else if (KEY_UP == ch) {
      if (the_frame[actual_window].index_current)
        the_frame[actual_window].index_current -= 1;
    } else if (KEY_DOWN == ch) {
      if (the_frame[actual_window].index_current <
          the_frame[actual_window].number_of_records - 1)
        the_frame[actual_window].index_current += 1;
    } else if (KEY_ENTER == ch || '\n' == ch) {
      if (the_frame[actual_window].index_current == 0) {
        if (strcmp(the_frame[actual_window].directory_name, "/"))
          change_directory_up(the_frame[actual_window].directory_name);
      } else {
        if (info_arrays[actual_window][the_frame[actual_window].index_current]
                .type == DT_DIR) {
          if (try_to_change_directory(
                  the_frame[actual_window].directory_name,
                  info_arrays[actual_window]
                             [the_frame[actual_window].index_current]
                                 .name) > 0) {
            change_directory(the_frame[actual_window].directory_name,
                             info_arrays[actual_window]
                                        [the_frame[actual_window].index_current]
                                            .name);
            the_frame[actual_window].index_current = 0;
          } else {
            wattron(stdscr, A_BLINK | A_BOLD);
            mvprintw(the_frame[0].rows, 0, "Cannot open folder <%s>",
                     info_arrays[actual_window]
                                [the_frame[actual_window].index_current]
                                    .name);
            wattroff(stdscr, A_BLINK | A_BOLD);
            while (getch() == '\n')
              ;
            move(the_frame[0].rows, 0);
            draw_horizontal_line(stdscr, the_frame[0].columns * 2 + 2, ' ');
          }
        }
      }
      if (get_dir_info(the_frame[actual_window].directory_name,
                       info_arrays[actual_window],
                       &(the_frame[actual_window].number_of_records)) < 0) {
        char str_err[LEN];
        sprintf(str_err, "%s %d", __FILE__, __LINE__);
        perror(str_err);
        int y, x;
        getyx(the_window[actual_window], y, x);

        move(the_frame[actual_window].rows - 1, 0);
        wprintw(the_window[actual_window], "##%d##", errno);
        if (errno == EACCES) wprintw(the_window[actual_window], "##EACCESS##");
        move(y, x);
      }
      wrefresh(the_window[actual_window]);
    }
    // ch = ' ';
    if (g_chahged_screen_size) {
      g_chahged_screen_size = 0;
      delwin(the_window[LEFT_WINDOW]);
      delwin(the_window[RIGHT_WINDOW]);
      erase();
      windows_initiation(the_window, the_frame);
    }
    output_a_window(the_window[actual_window], the_frame[actual_window],
                    info_arrays[actual_window]);
    ch = getch();
  }  // while ch != <ESC>
  delwin(the_window[LEFT_WINDOW]);
  delwin(the_window[RIGHT_WINDOW]);
  curs_set(FALSE);
  refresh();
  endwin();
  exit(EXIT_SUCCESS);
}