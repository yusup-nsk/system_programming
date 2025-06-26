#include <stdio.h>

#define MAX_LEN 256

void InputStr(char *str);
char *MyStrstr(const char *heystack, const char *needle);

int main() {
  char heystack[MAX_LEN], needle[MAX_LEN];
  InputStr(heystack);
  InputStr(needle);
  char *ptr = MyStrstr(heystack, needle);
  if (NULL == ptr) {
    printf("substring not found.\npointer points to NULL\n");
  } else {
    printf("pointer points to first substring in the string:\n");
    printf("%s\n", heystack);
    for (int i = 0; i < ptr - heystack; i++) putchar(' ');
    printf("^\n");
    for (int i = 0; i < ptr - heystack; i++) putchar(' ');
    printf("%s\n", needle);
  }
  return 0;
}

void InputStr(char *str) {
  fgets(str, MAX_LEN, stdin);
  char *p = str;
  for (; *p; p++)
    ;
  p--;
  if ('\n' == *p) *p = '\0';
}

char *MyStrstr(const char *heystack, const char *needle) {
  char *answer = NULL;
  const char *ptr = heystack;

  while (*ptr && NULL == answer) {
    for (; *ptr && *ptr != *needle; ptr++)
      ;
    if (*ptr) {
      const char *ptr_needle = needle;
      for (const char *p = ptr; *p && *p == *ptr_needle; p++, ptr_needle++)
        ;
      if ('\0' == *ptr_needle) {
        answer = (char *)ptr;
      } else
        ptr++;
    }
  }
  return answer;
}
