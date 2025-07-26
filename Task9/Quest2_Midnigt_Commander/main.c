// TODO при превышениии максимум файлов - переписать массив - файлы от 256 до
// 512

#include <curses.h>
#include <dirent.h>
#include <errno.h>
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

void change_directory_up(char *fulldirname) {
  if (strcmp(fulldirname, "/")) {
    char *p = strrchr(fulldirname, '/');
    if (p == fulldirname) {
      p[1] = '\0';
    } else
      p[0] = '\0';
  }
}

void change_directory(char *fulldirname, const char *directory) {
  if (strcmp(fulldirname, "/")) strncat(fulldirname, "/", 2);
  strncat(fulldirname, directory, LEN);
}

void initiation_windows(WINDOW *the_window[2], unsigned rows[2],
                        unsigned cols[2]) {
  struct winsize size;
  unsigned pin_x[2], pin_y[2];
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  rows[LEFT_WINDOW] = rows[RIGHT_WINDOW] = size.ws_row - 1;
  cols[LEFT_WINDOW] = cols[RIGHT_WINDOW] = size.ws_col / 2 - 1;
  pin_x[LEFT_WINDOW] = 0;
  pin_x[RIGHT_WINDOW] = cols[LEFT_WINDOW] + pin_x[LEFT_WINDOW] + 2;
  pin_y[LEFT_WINDOW] = pin_y[RIGHT_WINDOW] = 0;
  for (int i = 0; i < 2; ++i) {
    the_window[i] = newwin(rows[i], cols[i], pin_y[i], pin_x[i]);
    wbkgd(the_window[i], COLOR_PAIR(2));
  }
}

typedef struct {
  char directory_name[LEN];
  unsigned number_of_records;
  unsigned index_current;
  unsigned rows;
  unsigned columns;
} Frame;

int main() {
  WINDOW *the_window[2], win;
  Frame the_frame[2], frame;
  unsigned rows[2], cols[2];
  Info info_arrays[2][MAX_FILES];
  // Info header = {" ", " "}
  char dir_name[2][LEN] = {"/", "/"};
  unsigned num_records[2];
  int actual_window = LEFT_WINDOW;
  unsigned current_index[2] = {0, 0};

  freopen("/dev/nul", "w", stderr);
  initscr();
  // chtype chtp;
  signal(SIGWINCH, sig_winch);
  curs_set(FALSE);
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  refresh();
  init_pair(1, COLOR_BLUE, COLOR_GREEN);
  init_pair(2, COLOR_YELLOW, COLOR_BLUE);

  initiation_windows(the_window, rows, cols);
  for (int i = 0; i < 2; ++i) {
    get_dir_info(dir_name[i], info_arrays[i], &num_records[i]);
  }
  int ch = ' ';
  while (ch != 'q' && ch != 'Q') {
    if (9 == ch) {  // TAB
      actual_window = !actual_window;
    } else if (KEY_UP == ch) {
      if (current_index[actual_window]) current_index[actual_window] -= 1;
      //  mvprintw(rows[actual_window] - 2, cols[actual_window] + 1, "--%u--\n",
      //      current_index[actual_window]);
    } else if (KEY_DOWN == ch) {
      if (current_index[actual_window] < num_records[actual_window] - 1)
        current_index[actual_window] += 1;
      //   mvprintw(rows[actual_window], cols[actual_window] + 1, "++%u++\n",
      //    current_index[actual_window]);
    } else if (KEY_ENTER == ch || '\n' == ch) {
      if (current_index[actual_window] == 0) {
        if (strcmp(dir_name[actual_window], "/"))
          change_directory_up(dir_name[actual_window]);
      } else {
        if (info_arrays[actual_window][current_index[actual_window]].type ==
            DT_DIR) {
          change_directory(
              dir_name[actual_window],
              info_arrays[actual_window][current_index[actual_window]].name);
          current_index[actual_window] = 0;
        }
      }
      if (get_dir_info(dir_name[actual_window], info_arrays[actual_window],
                       &num_records[actual_window]) < 0) {
        char str_err[LEN];
        sprintf(str_err, "%s %d", __FILE__, __LINE__);
        perror(str_err);
        // exit(EXIT_FAILURE);
        mvprintw(10, 45, "##%d##", errno);
        if (errno == EACCES) mvprintw(11, 45, "##EACCESS##");
      }
      // wprintw(the_window[actual_window], "==%s==\n",
      // dir_name[actual_window]);
      wrefresh(the_window[actual_window]);
      //  sleep(1);
    }
    // mvprintw(rows[actual_window] - 1, 1, "num_records==%u;   current==%u\n",
    //          num_records[actual_window], current_index[actual_window]);

    ch = ' ';
    if (g_chahged_screen_size) {
      g_chahged_screen_size = 0;
      delwin(the_window[LEFT_WINDOW]);
      delwin(the_window[RIGHT_WINDOW]);
      erase();
      initiation_windows(the_window, rows, cols);
    }
    werase(the_window[actual_window]);
    wmove(the_window[actual_window], 0, 0);
    draw_horizontal_line(the_window[actual_window], cols[actual_window], '-');
    wmove(the_window[actual_window], 0, 0);
    wprintw(
        the_window[actual_window], "<-...%s",
        make_short_dirname(dir_name[actual_window], cols[actual_window] - 7));
    wmove(the_window[actual_window], 1, 0);
    output_to_window(the_window[actual_window], info_arrays[actual_window],
                     num_records[actual_window], rows[LEFT_WINDOW] - 5,
                     cols[LEFT_WINDOW] - 4, current_index[actual_window]);
    draw_horizontal_line(the_window[actual_window], cols[actual_window], '-');

    wprintw(the_window[actual_window], "%s\n",
            info_arrays[actual_window][current_index[actual_window]].name);
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