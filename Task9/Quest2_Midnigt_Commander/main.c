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

typedef struct {
  char directory_name[LEN];
  unsigned number_of_records;
  unsigned index_current;
  unsigned rows;
  unsigned columns;
} Frame;

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

void windows_initiation(WINDOW *the_window[2], Frame the_frame[2]) {
   struct winsize size;
  unsigned pin_x[2], pin_y[2];
  unsigned rows[2], cols[2];
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  rows[LEFT_WINDOW] = rows[RIGHT_WINDOW] = size.ws_row - 1;
  cols[LEFT_WINDOW] = cols[RIGHT_WINDOW] = size.ws_col / 2 - 1;
  pin_x[LEFT_WINDOW] = 0;
  pin_x[RIGHT_WINDOW] = cols[LEFT_WINDOW] + pin_x[LEFT_WINDOW] + 2;
  pin_y[LEFT_WINDOW] = pin_y[RIGHT_WINDOW] = 0;
  for (int i = 0; i < 2; ++i) {
    the_frame[i].index_current = 0;
    the_frame[i].rows = rows[i];
    the_frame[i].columns = cols[i];
    the_window[i] = newwin(rows[i], cols[i], pin_y[i], pin_x[i]);
    wbkgd(the_window[i], COLOR_PAIR(1));
  }
}

int main() {
  WINDOW *the_window[2], win;
  Frame the_frame[2], frame;
  Info info_arrays[2][MAX_FILES];
  strncpy(the_frame[LEFT_WINDOW].directory_name, "/", 2);
  strncpy(the_frame[RIGHT_WINDOW].directory_name, "/", 2);
  int actual_window = LEFT_WINDOW;
 // freopen("/dev/nul", "w", stderr);
  initscr();
  // chtype chtp;
  signal(SIGWINCH, sig_winch);
  curs_set(FALSE);
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  refresh();
  init_pair(1, COLOR_YELLOW, COLOR_BLUE);
  init_pair(2, COLOR_YELLOW, COLOR_BLUE);

  windows_initiation(the_window, the_frame);
  for (int i = 0; i < 2; ++i) {
    get_dir_info(the_frame[i].directory_name, info_arrays[i],
                 &(the_frame[i].number_of_records));
  }
  // int ch = ' ';
   chtype ch = ' ';
  while (ch != 'q' && ch != 'Q') {
    if (9 == ch) {  // TAB
      actual_window = !actual_window;
    } else if (KEY_UP == ch) {
      if (the_frame[actual_window].index_current) the_frame[actual_window].index_current -= 1;
    } else if (KEY_DOWN == ch) {
      if (the_frame[actual_window].index_current < the_frame[actual_window].number_of_records - 1)
       the_frame[actual_window].index_current += 1;
    } else if (KEY_ENTER == ch || '\n' == ch) {
      if (the_frame[actual_window].index_current == 0) {
        if (strcmp(the_frame[actual_window].directory_name , "/"))
           change_directory_up(the_frame[actual_window].directory_name);
      } else {
        if (info_arrays[actual_window][the_frame[actual_window].index_current].type ==
            DT_DIR) {
                 change_directory(
              the_frame[actual_window].directory_name, 
              info_arrays[actual_window][the_frame[actual_window].index_current].name);
 
          the_frame[actual_window].index_current = 0;
        }
      }
      if (get_dir_info(the_frame[actual_window].directory_name, info_arrays[actual_window],
                       &(the_frame[actual_window].number_of_records)) < 0) {
        char str_err[LEN];
        sprintf(str_err, "%s %d", __FILE__, __LINE__);
        perror(str_err);
        mvprintw(10, 45, "##%d##", errno);
        if (errno == EACCES) mvprintw(11, 45, "##EACCESS##");
      }
      wrefresh(the_window[actual_window]);
    }

    ch = ' ';
    if (g_chahged_screen_size) {
      g_chahged_screen_size = 0;
      delwin(the_window[LEFT_WINDOW]);
      delwin(the_window[RIGHT_WINDOW]);
      erase();
      windows_initiation(the_window, the_frame);
    }
    werase(the_window[actual_window]);
    wmove(the_window[actual_window], 0, 0);
    draw_horizontal_line(the_window[actual_window], the_frame[actual_window].columns, '-');
    wmove(the_window[actual_window], 0, 0);
    wprintw(
        the_window[actual_window], "<-...%s",
        make_short_dirname(the_frame[actual_window].directory_name, the_frame[actual_window].columns - 7));
    wmove(the_window[actual_window], 1, 0);
    output_to_window(the_window[actual_window], info_arrays[actual_window],
                    the_frame[actual_window].number_of_records, the_frame[actual_window].rows- 5,
                    the_frame[actual_window].columns - 4, the_frame[actual_window].index_current);
    draw_horizontal_line(the_window[actual_window], the_frame[actual_window].columns, '-');

    wprintw(the_window[actual_window], "%s\n",
            info_arrays[actual_window][the_frame[actual_window].index_current].name);
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