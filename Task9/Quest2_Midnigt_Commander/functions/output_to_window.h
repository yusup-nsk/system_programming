#ifndef _OUTPUT_TO_WINDOW_H_
#define _OUTPUT_TO_WINDOW_H_

#include <curses.h>
#include <termios.h>

#include "get_dir_info.h"

int make_string_for_size_column(char *str_size, unsigned size_len,
                                unsigned size);
void draw_horizontal_line(WINDOW *window, unsigned columns, char symbol);
const char *make_short_dirname(const char *fullname, unsigned length);
void output_the_win(WINDOW *win, Frame frame, const Info *info,
                    unsigned actual);
#endif