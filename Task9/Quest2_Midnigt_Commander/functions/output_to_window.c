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

void output_to_window(WINDOW *wnd, const Info *arr_info, unsigned number,
                      unsigned rows, unsigned columns, unsigned current_index) {
  if (NULL == arr_info || 0 == number || 0 == rows) return;
  unsigned min_name_len = 12, min_size_len = 7, min_time_len = 12;
  unsigned name_len = 12, size_len = 7, time_len = 12;
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
  char str_size[T_LEN];
  sprintf(format, " %%-%u.%us|%%%u.%us|%%-%u.%us\n", name_len, name_len,
          size_len, size_len, time_len, time_len);
  unsigned n = 0;
  if (current_index > rows - 1) {
    n = current_index - rows + 1;
    for (; n <= current_index; n++) {
      symbols =
          make_string_for_size_column(str_size, size_len, arr_info[n].size);
      format[0] = (arr_info[n].type == DT_DIR) ? '/' : ' ';
      symbols =
          wprintw(wnd, format, arr_info[n].name, str_size, arr_info[n].time);
      assert(symbols <= columns + 1);
    }
  } else {
    for (; n < number && n < rows; n++) {
      symbols =
          make_string_for_size_column(str_size, size_len, arr_info[n].size);
      format[0] = (arr_info[n].type == DT_DIR) ? '/' : ' ';
      symbols =
          wprintw(wnd, format, arr_info[n].name, str_size, arr_info[n].time);
      // assert(symbols <= columns + 1);
    }
    if (number < rows) {
      for (; n < rows; n++) {
        symbols =
            make_string_for_size_column(str_size, size_len, arr_info[n].size);
        format[0] = (arr_info[n].type == DT_DIR) ? '/' : ' ';
        symbols = wprintw(wnd, format, " ", " ", " ");
        // assert(symbols <= columns + 1);
      }
    }
  }
  wprintw(wnd, "format: %s;   symbols==%d", format, symbols);
}
