#ifndef _OUTPUT_TO_WINDOW_H_
#define _OUTPUT_TO_WINDOW_H_

#include <curses.h>
#include <termios.h>

#include "get_dir_info.h"

int make_string_for_size_column(char *str_size, unsigned size_len,
                                unsigned size);
void output_to_window(WINDOW *, const Info *arr_info, unsigned number,
                      unsigned rows, unsigned columns, unsigned current_index);
#endif