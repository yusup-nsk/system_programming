#include "get_dir_info.h"

int my_filter(const struct dirent *name) { return strcmp(name->d_name, "."); }

int my_compar(const struct dirent **de1, const struct dirent **de2) {
  if (strcmp((*de1)->d_name, "..") == 0) return -1;
  if (strcmp((*de2)->d_name, "..") == 0) return 1;
  int res;
  if ((*de1)->d_type == DT_DIR) {
    if ((*de2)->d_type == DT_DIR) {
      res = strcmp((*de1)->d_name, (*de2)->d_name);
    } else
      res = -1;
  } else if ((*de2)->d_type == DT_DIR) {
    res = 1;
  } else
    res = strcmp((*de1)->d_name, (*de2)->d_name);
  return res;
}

void make_full_filename(char *fulldirname, char *name, char *full_filename) {
  if (strcmp(name, "/")) {
    strncpy(full_filename, fulldirname, LEN);
    strncat(full_filename, "/", 2);
    strncat(full_filename, name, LEN);
  } else
    strncpy(full_filename, fulldirname, LEN);
}

int try_to_change_directory(char *fulldirname, const char *directory) {
  char str[LEN];
  strncpy(str, fulldirname, LEN);
  strncat(str, "/", 2);
  strncat(str, directory, LEN);
  struct dirent **namelist;
  return scandir(str, &namelist, my_filter, my_compar);
}

int get_dir_info(const char *fulldirname, Info *arr_info,
                 unsigned *ptr_number) {
  *ptr_number = 0;
  struct dirent **namelist;
  int number = scandir(fulldirname, &namelist, my_filter, my_compar);
  if (number == -1) {
    char str_err[LEN];
    sprintf(str_err, "scandir error in directory %s", fulldirname);
    perror(str_err);
    return -1;
  }
  if (number > MAX_FILES) number = MAX_FILES;
  int n = 0;
  for (; n < number; n++) {
    time_t time_now = time(NULL);
    struct tm *now = localtime(&time_now);
    int year_now = now->tm_year;
    char str_time[T_LEN];
    char s[LEN];
    strncpy(s, fulldirname, LEN);
    strncat(s, "/", 2);
    strncat(s, namelist[n]->d_name, LEN);
    struct stat file_stat;
    if (stat(s, &file_stat) == -1) {
      strftime(str_time, sizeof(str_time), "%d %h %H:%M", now);
      strncpy(arr_info[n].name, namelist[n]->d_name, LEN);
      strncpy(arr_info[n].time, str_time, LEN);
      arr_info[n].size = 0;
      arr_info[n].type = namelist[n]->d_type;
      continue;
    }
    time_t mtime = file_stat.st_mtim.tv_sec;
    struct tm *stime = localtime(&mtime);
    if (year_now == stime->tm_year) {
      strftime(str_time, sizeof(str_time), "%d %h %H:%M", stime);
    } else {
      strftime(str_time, sizeof(str_time), "%d %h %Y", stime);
    }
    strncpy(arr_info[n].name, namelist[n]->d_name, LEN);
    strncpy(arr_info[n].time, str_time, LEN);
    arr_info[n].size = file_stat.st_size;
    arr_info[n].type = namelist[n]->d_type;
  }
  *ptr_number = number;
  for (int i = 0; i < number; i++) free(namelist[i]);
  free(namelist);
  return n;
}

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
  mvprintw(size.ws_row - 1, 0, "Press <Esc> for exit");
  wattroff(stdscr, A_BOLD);
  if (size.ws_row<10){
    rows[LEFT_WINDOW] = rows[RIGHT_WINDOW] = MINIMUM_ROWS -2;
  } else {
     rows[LEFT_WINDOW] = rows[RIGHT_WINDOW] = size.ws_row - 2;
  }
  if (size.ws_col <38){
    cols[LEFT_WINDOW] = cols[RIGHT_WINDOW] = MINIMUM_COLUMNS/2-1;
  } else {
      cols[LEFT_WINDOW] = cols[RIGHT_WINDOW] = size.ws_col / 2 - 1;
  }
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
