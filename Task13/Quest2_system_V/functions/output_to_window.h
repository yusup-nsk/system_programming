#ifndef _OUTPUT_TO_WINDOW_H_
#define _OUTPUT_TO_WINDOW_H_

#include <curses.h>
#include <termios.h>

#include "windows_procedures.h"

int make_string_for_size_column(char *str_size, unsigned size_len,
                                unsigned size);
void draw_horizontal_line(WINDOW *window, unsigned columns, char symbol);
void output_the_win(WINDOW *win, Frame frame, unsigned actual);

void process_change_screen_size(WINDOW **window, Frame *frame, unsigned actual);
#endif
