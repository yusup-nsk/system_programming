#include "functions.h"

int input_arr(char ***arr, unsigned *count) {
  if (!(arr && count)) {
    return -1;
  }
  *count = 0;
  printf("Input command: ");
  char line[MAX_LINE];
  unsigned len;
  int input_res = input_string(line, &len);
  if (-1 == input_res) {
    return 0;
  } else if (-2 == input_res) {
    // printf("Wrong  input\n");
    return -2;
  }

  printf("\n\nline:<%s>\n", line);

  unsigned sz = SZ;
  *arr = (char **)malloc(sz * sizeof(char *));
  if (NULL == *arr) {
    printf("Failed malloc for array\n");
    return -1;
  }
  char delimeters[] = " \t\n";
  for (char *s = strtok(line, delimeters); s; s = strtok(NULL, delimeters)) {
    if (sz < *count + 3) {
      sz *= 2;
      char **tmp = (char **)realloc(*arr, sz * sizeof(char *));
      assert(tmp);
      *arr = tmp;
    }
    (*arr)[*count] = (char *)malloc(LEN);
    char format[LEN];
    sprintf(format, "%%%ds", LEN - 1);
    sscanf(s, format, (*arr)[*count]);
    (*arr)[*count][LEN - 1] = 0;
    *count += 1;
  }
  if (strncmp((*arr)[0], "|", 2) == 0) {
    return -2;
  }
  for (unsigned i = 0; i < *count - 1; ++i) {
    if (strncmp((*arr)[i], "|", 2) == 0 &&
        strncmp((*arr)[i + 1], "|", 2) == 0) {
      return -3;
    }
  }


  if (strncmp((*arr)[*count - 1], "|", 2) == 0) {
    (*arr)[*count - 1] = NULL;

  } else {
    (*arr)[*count] = NULL;
    *count += 1;
  }
  return sz;  // or return count;
}

void free_arr(char **arr, unsigned count) {
  if (arr) {
    for (unsigned i = 0; i < count; ++i) {
      if (arr[i]) {
        free(arr[i]);
      }
    }
    free(arr);
  }
}

// void output_arr(char **arr, unsigned count) {
//   if (arr) {
//     for (unsigned i = 0; i < count; ++i) {
//       if (arr[i]) {
//         printf("%u:  %s\n", i, arr[i]);
//       } else
//         printf("%u:  %s\n", i, "NULL");
//     }
//   }
// }

int input_string(char *line, unsigned *len) {
  char scanned_line[MAX_LINE] = {0};
  fgets(scanned_line, MAX_LINE, stdin);
  if (0 == scanned_line[0] || '\n' == scanned_line[0]) {
    *len = 0;
    return -1;
  }
  scanned_line[MAX_LINE - 1] = 0;
  unsigned line_index = 0;
  if ('|' == scanned_line[0]) {
    return -2;
    // scanned_line[0] = ' ';
  }
  line[line_index++] = scanned_line[0];
  for (int i = 1; i < MAX_LINE && line_index < MAX_LINE; i++) {
    if ('|' == scanned_line[i]) {
      if (scanned_line[i - 1] != ' ') {
        line[line_index++] = ' ';
      }
      line[line_index++] = '|';
      if (scanned_line[i + 1] != ' ') {
        line[line_index++] = ' ';
      }
    } else {
      line[line_index++] = scanned_line[i];
    }
  }
  line[MAX_LINE - 1] = '\0';
  *len = line_index;
  return 0;
}

int get_token(char **arr, char ***token, unsigned *index_in_arr) {
  if (!(arr)) {
    return -1;
  }
  int count = 0;
  char **ptr = arr + *index_in_arr;
  for (; ptr && *ptr; ptr++, count++) {
    if (strncmp(*ptr, "|", 2) == 0) {
      break;
    }
  }
  if (0 == count) {
    return 0;
  }
  count++;
  *token = (char **)malloc((count) * sizeof(char *));
  for (int i = 0; i < count - 1; i++) {
    unsigned size = strlen(arr[*index_in_arr + i]) + 1;
    (*token)[i] = (char *)malloc(size * sizeof(char));
    memcpy((*token)[i], arr[*index_in_arr + i], size);
  }
  (*token)[count - 1] = NULL;
  // output_arr(*token, count);
  *index_in_arr += count;
  return count;
}