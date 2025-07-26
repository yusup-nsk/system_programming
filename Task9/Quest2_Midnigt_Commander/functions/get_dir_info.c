#include "get_dir_info.h"

void make_full_filename(char *fulldirname, char *name, char *full_filename) {
  // char  full_filename[LEN];
  if (strcmp(name, "/")) {
    strncpy(full_filename, fulldirname, LEN);
    strncat(full_filename, "/", 2);
    strncat(full_filename, name, LEN);
    //  sprintf(full_filename,  "%s/%s", fulldirname, full_filename);
  } else
    strncpy(full_filename, fulldirname, LEN);
}

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
    // exit(EXIT_FAILURE);
  }
  if (number > MAX_FILES) number = MAX_FILES;
  //*ptr_number = number;
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
    // int fstat =
    if (stat(s, &file_stat) == -1) {
      // char str_err[LEN] = "error in stat() for directory ";
      // strncat(str_err, s, LEN);
      // perror(str_err);
      // perror(s);

      // continue;
      // return -1;

      strftime(str_time, sizeof(str_time), "%d %h %H:%M", now);
      strncpy(arr_info[n].name, namelist[n]->d_name, LEN);
      strncpy(arr_info[n].time, str_time, LEN);
      arr_info[n].size = 0;  // file_stat.st_size;
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
    //  *ptr_number += 1;
  }
  *ptr_number = number;
  for (int i = 0; i < number; i++) free(namelist[i]);
  free(namelist);
  return n;
}
