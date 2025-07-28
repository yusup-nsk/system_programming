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
        size = size >> 10;
        symbols = sprintf(str_size, "~G");
      }
    }
  }
  return symbols;
}

unsigned output_string(WINDOW *wnd, char *format, unsigned size_len,
                       Info info) {
  char str_size[LEN];
  unsigned symbols;
  make_string_for_size_column(str_size, size_len, info.size);
  format[0] = (info.type == DT_DIR) ? '/' : ' ';
  symbols = wprintw(wnd, format, info.name, str_size, info.time);
  return symbols;
}

void draw_horizontal_line(WINDOW *window, unsigned columns, char symbol) {
  for (unsigned i = 0; i < columns; i++) {
    wprintw(window, "%c", symbol);
  }
}

const char *make_short_dirname(const char *fullname, unsigned length) {
  unsigned length_full = strlen(fullname);
  const char *result;
  if (length >= length_full) {
    result = fullname;
  } else {
    result = fullname + length_full - length;
  }
  return result;
}

void output_info_frame(WINDOW *wnd, const Info *arr_info, Frame frame,
                       unsigned actual) {
  unsigned number = frame.number_of_records;
  unsigned rows = frame.rows - 5;
  unsigned columns = frame.columns - 1;
  unsigned current_index = frame.index_current;
  if (NULL == arr_info || 0 == number || 0 == rows) return;
  unsigned min_name_len = 12, min_size_len = 7, min_time_len = 12;
  unsigned name_len = 12, size_len = 7, time_len = 12;
  wattroff(wnd, A_BOLD);
  if (name_len + size_len + time_len <= columns - 3) {
    name_len = columns - 3 - size_len - time_len;
  } else {
    size_len =
        ((columns - 3.0) * 7.0 / (min_name_len + min_size_len + min_time_len));
    time_len =
        ((columns - 3.0) * 12.0 / (min_name_len + min_size_len + min_time_len));
    name_len = columns - 3 - size_len - time_len;
  }
  if (current_index > number - 1) current_index = number - 1;
  char format[LEN];
  unsigned symbols;
  sprintf(format, " %%-%u.%us|%%%u.%us|%%-%u.%us\n", name_len, name_len,
          size_len, size_len, time_len, time_len);
  wattron(wnd, A_BOLD);
  wprintw(wnd, format, " NAME ", " SIZE ", " TIME ");
  wattroff(wnd, A_BOLD);
  unsigned n = 0;
  if (current_index > rows - 1) {
    n = current_index - rows + 1;
    for (; n < current_index; n++) {
      symbols = output_string(wnd, format, size_len, arr_info[n]);
      assert(symbols <= columns + 1000);
    }
    if (actual) {
      wattron(wnd, A_BOLD);
      symbols = output_string(wnd, format, size_len, arr_info[n]);
      wattroff(wnd, A_BOLD);
      assert(symbols <= columns + 1000);
    } else {
      output_string(wnd, format, size_len, arr_info[n]);
    }
  } else {
    for (; n < number && n < rows; n++) {
      if (n == current_index && actual) {
        wattron(wnd, A_BOLD);
        symbols = output_string(wnd, format, size_len, arr_info[n]);
        wattroff(wnd, A_BOLD);
      } else {
        symbols = output_string(wnd, format, size_len, arr_info[n]);
      }
      assert(symbols <= columns + 100);
    }
    if (number < rows) {
      for (; n < rows; n++) {
        format[0] = ' ';
        symbols = wprintw(wnd, format, " ", " ", " ");
        assert(symbols <= columns + 100);
      }
    }
  }
}

void output_the_win(WINDOW *win, Frame frame, const Info *info,
                    unsigned actual) {
  werase(win);
  wmove(win, 0, 0);
  draw_horizontal_line(win, frame.columns, '-');
  wmove(win, 0, 0);
  if (actual) {
    wattron(win, A_BOLD);
    wprintw(win, "<-...%s",
            make_short_dirname(frame.directory_name, frame.columns - 6));
    wattroff(win, A_BOLD);
  } else {
    wprintw(win, "<-...%s",
            make_short_dirname(frame.directory_name, frame.columns - 6));
  }
  wmove(win, 1, 0);
  output_info_frame(win, info, frame, actual);
  draw_horizontal_line(win, frame.columns, '-');
  if (frame.index_current) {
    wprintw(win, "%s\n", info[frame.index_current].name);
  } else {
    wprintw(win, "--UP--\n");
  }
  wrefresh(win);
}