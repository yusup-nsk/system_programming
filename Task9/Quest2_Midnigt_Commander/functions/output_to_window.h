#ifndef _OUTPUT_TO_WINDOW_H_
#define _OUTPUT_TO_WINDOW_H_

#include <curses.h>
#include <termios.h>

#include "get_dir_info.h"

int make_string_for_size_column(char *str_size, unsigned size_len,
                                unsigned size);
// void output_info(WINDOW *, const Info *arr_info, unsigned number, unsigned
// rows,
//                  unsigned columns, unsigned current_index);
void draw_horizontal_line(WINDOW *window, unsigned columns, char symbol);

const char *make_short_dirname(const char *fullname, unsigned length);
void output_a_window(WINDOW *win, Frame frame, const Info *info);
#endif