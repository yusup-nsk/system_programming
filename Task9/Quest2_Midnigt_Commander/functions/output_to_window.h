#ifndef _OUTPUT_TO_WINDOW_H_
#define _OUTPUT_TO_WINDOW_H_

#include <curses.h>
#include <termios.h>

#include "get_dir_info.h"

int make_string_for_size_column(char *str_size, unsigned size_len,
                                unsigned size);
void draw_horizontal_line(WINDOW *window, unsigned columns, char symbol);
void output_info_frame(WINDOW *wnd, const Info *arr_info, Frame frame,
                       unsigned actual);
const char *make_short_dirname(const char *fullname, unsigned length);
void output_the_win(WINDOW *win, Frame frame, const Info *info,
                    unsigned actual);
void enter_to_directory(Frame *frame, Info *info);
void process_change_screen_size(WINDOW **window, Frame *frame,
                                Info info[2][MAX_FILES], unsigned actual);
#endif