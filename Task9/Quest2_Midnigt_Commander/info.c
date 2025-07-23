#define _DEFAULT_SOURCE
#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define T_LEN 13
#define LEN 128
#define MAX_FILES 256

typedef struct {
  char name[LEN];
  size_t size;
  char time[T_LEN];
  int type;
} Info;

int filter(const struct dirent *name);
int compar(const struct dirent **de1, const struct dirent **de2);
int dir_info(const char fulldirname[LEN], Info *arr_info);

int make_string_for_size_column(char *str_size, unsigned size_len,
                                unsigned size);
void output_info(Info *arr_info, int number);
void output_info_w(const Info *arr_info, unsigned number, unsigned rows,
                   unsigned columns, unsigned current_index);

void change_directory_up(char *fulldirname);
void change_directory(char *fulldirname, const char *directory);

int main(int argc, char *argv[]) {
  Info arr_info[MAX_FILES];

  char fulldirname[LEN] = "/";

  int number = dir_info(fulldirname, arr_info);
  output_info(arr_info, number);
  unsigned current_index = 10;
  output_info_w(arr_info, number, 6, 40, current_index);

  // printf("\n%s\n", fulldirname);
  // change_directory_up(fulldirname);

  printf("\n%s\n", fulldirname);
  if (arr_info[1].type == DT_DIR) {
    change_directory(fulldirname, arr_info[1].name);
    printf("\n%s\n", fulldirname);
    number = dir_info(fulldirname, arr_info);
    output_info_w(arr_info, number, 6, 40, 100);
  }

  exit(EXIT_SUCCESS);
}

int filter(const struct dirent *name) { return strcmp(name->d_name, "."); }

int compar(const struct dirent **de1, const struct dirent **de2) {
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

int dir_info(const char fulldirname[LEN], Info *arr_info) {
  struct dirent **namelist;
  int number = scandir(fulldirname, &namelist, filter, compar);
  if (number == -1) {
    perror("scandir");
    exit(EXIT_FAILURE);
  }
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
    int fstat = stat(s, &file_stat);
    time_t mtime = file_stat.st_mtim.tv_sec;
    struct tm *stime = localtime(&mtime);

    if (year_now == stime->tm_year) {
      strftime(str_time, sizeof(str_time), "%d %h %H:%M", stime);
    } else {
      strftime(str_time, sizeof(str_time), "%d %h %Y", stime);
    }
    // printf("%s\n%*s  | %*lu |  %*s  \n\n", s, 20, namelist[n]->d_name, 10,
    //        file_stat.st_size, 20, str);
    strncpy(arr_info[n].name, namelist[n]->d_name, LEN);
    strncpy(arr_info[n].time, str_time, LEN);
    arr_info[n].size = file_stat.st_size;
    arr_info[n].type = namelist[n]->d_type;
  }
  for (int n = 0; n < number; n++) free(namelist[n]);
  free(namelist);
  return number;
}

void change_directory_up(char *fulldirname) {
  if (strcmp(fulldirname, "/")) {
    char *p = strrchr(fulldirname, '/');
    *p = '\0';
  }
}
void change_directory(char *fulldirname, const char *directory) {
  strncat(fulldirname, "/", 2);
  strncat(fulldirname, directory, LEN);
}

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
  int symbols = sprintf(str_size, "%u", size);
  if (symbols > size_len) {
    // size /= 1024;
    size = size >> 10;
    symbols = sprintf(str_size, "%uK", size);
    if (symbols > size_len) {
      // size /= 1024;
      size = size >> 10;
      symbols = sprintf(str_size, "%uM", size);
      if (symbols > size_len) {
        // size /= 1024;
        size = size >> 10;
        if (size)
          symbols = sprintf(str_size, "%uG", size);
        else
          symbols = sprintf(str_size, "~G");
      }
      if (symbols > size_len) {
        // size /= 1024;
        size = size >> 10;
        symbols = sprintf(str_size, "~G");
      }
    }
  }
  return symbols;
}

void output_info(Info *arr_info, int number) {
  for (int n = 0; n < number; n++) {
    printf("%-*s|%*lu|%-*.20s\n", 20, arr_info[n].name, 10, arr_info[n].size,
           20, arr_info[n].time);
  }
}

void output_info_w(const Info *arr_info, unsigned number, unsigned rows,
                   unsigned columns, unsigned current_index) {
  if (NULL == arr_info || 0 == number || 0 == rows) return;
  unsigned min_name_len = 12, min_size_len = 7, min_time_len = 12;
  unsigned name_len = 12, size_len = 7, time_len = 12;
  if (name_len + size_len + time_len <= columns - 3) {
    name_len = columns - 3 - size_len - time_len;
  } else {
    size_len = ((columns - 3.0) * 7.0 / (12 + 7 + 12.));
    time_len = ((columns - 3.0) * 12.0 / (12 + 7 + 12.));
    name_len = columns - 3 - size_len - time_len;
  }
  if (current_index > number - 1) current_index = number - 1;
  char format[LEN];
  int symbols;
  char str_size[T_LEN];
  sprintf(format, " %%-%u.%us|%%%u.%us|%%-%u.%us\n", name_len, name_len,
          size_len, size_len, time_len, time_len);
  int n = 0;
  if (current_index > rows - 1) {
    n = current_index - rows + 1;
    for (; n <= current_index; n++) {
      symbols =
          make_string_for_size_column(str_size, size_len, arr_info[n].size);
      format[0] = (arr_info[n].type == DT_DIR) ? '/' : ' ';
      symbols = printf(format, arr_info[n].name, str_size, arr_info[n].time);
      assert(symbols <= columns + 1);
    }
  } else {
    for (; n < number && n < rows; n++) {
      symbols =
          make_string_for_size_column(str_size, size_len, arr_info[n].size);
      format[0] = (arr_info[n].type == DT_DIR) ? '/' : ' ';
      symbols = printf(format, arr_info[n].name, str_size, arr_info[n].time);
      assert(symbols <= columns + 1);
    }
    if (number < rows) {
      for (; n < rows; n++) {
        symbols =
            make_string_for_size_column(str_size, size_len, arr_info[n].size);
        format[0] = (arr_info[n].type == DT_DIR) ? '/' : ' ';
        symbols = printf(format, " ", " ", " ");
        assert(symbols <= columns + 1);
      }
    }
  }
  printf("format: %s;   symbols==%d", format, symbols);
}
