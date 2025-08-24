#include "output_to_window.h"

int make_string_for_size_column(char *str_size, unsigned size_len,
                                unsigned size) {
  if (0 == size_len) {
    str_size[0] = '\0';
    return 0;
  }
  if (1 == size_len) {
    if (size < 512)
      sprintf(str_size, "B");
    else if (size < (1 << 19))
      sprintf(str_size, "K");
    else if (size < (1 << 29))
      sprintf(str_size, "M");
    else
      sprintf(str_size, "G");
    return 1;
  }
  unsigned symbols = sprintf(str_size, "%u", size);
  if (symbols > size_len) {
    size = size >> 10;
    symbols = sprintf(str_size, "%uK", size);
    if (symbols > size_len) {
      size = size >> 10;
      symbols = sprintf(str_size, "%uM", size);
      if (symbols > size_len) {
        size = size >> 10;
        if (size)
          symbols = sprintf(str_size, "%uG", size);
        else
          symbols = sprintf(str_size, "~G");
      }
      if (symbols > size_len) {
        symbols = sprintf(str_size, "~G");
      }
    }
  }
  return symbols;
}

void draw_horizontal_line(WINDOW *window, unsigned columns, char symbol) {
  for (unsigned i = 0; i < columns; i++) {
    wprintw(window, "%c", symbol);
  }
}

// void output_the_win(WINDOW *win, Frame frame, unsigned actual) {
//   werase(win);
//   wmove(win, 0, 0);
//   // draw_horizontal_line(win, frame.columns, '-');
//   wmove(win, 0, 0);
//   if (actual) {
//     wattron(win, A_BOLD);
//     // wprintw(win, "<-...%s", "VVVVVVVVVVVVV");
//     wattroff(win, A_BOLD);
//   } else {
//     // wprintw(win, "<-...%s", "BBBBBBBBBBBBBBBB");
//   }
//   wmove(win, 1, 0);
//   // draw_horizontal_line(win, frame.columns, '-');
//   if (frame.index_current) {
//     // wprintw(win, "%s\n", "NNNNNNNNNNNN");
//   } else {
//     // wprintw(win, "--UP--\n");
//   }
//   wrefresh(win);
// }
