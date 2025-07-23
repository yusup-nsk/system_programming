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

int get_dir_info(const char *fulldirname, Info *arr_info,
                 unsigned *ptr_number) {
  struct dirent **namelist;
  int number = scandir(fulldirname, &namelist, my_filter, my_compar);
  if (number == -1) {
    perror("scandir");
    exit(EXIT_FAILURE);
  }
  *ptr_number = number;
  for (int n = 0; n < number; n++) {
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
    stat(s, &file_stat);
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
  for (int n = 0; n < number; n++) free(namelist[n]);
  free(namelist);
  return number;
}
