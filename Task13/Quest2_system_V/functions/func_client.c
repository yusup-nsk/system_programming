#include "func_client.h"

#include <unistd.h>

void make_random_name(char *name, unsigned length) {
  assert(length > 2);
  srand(time(NULL));
  char ch = 'A' + rand() % 26;
  name[0] = ch;
  for (unsigned i = 1; i < length - 1; i++) {
    ch = 'a' + rand() % 26;
    name[i] = ch;
  }
  name[length - 1] = '\0';
}

void make_random_name_len(char *name, unsigned max_length) {
  assert(max_length > 5);
  srand(time(NULL));
  char ch = 'A' + rand() % 26;
  name[0] = ch;
  unsigned len = 5 + rand() % (max_length - 3);
  for (unsigned i = 1; i < len - 1; i++) {
    ch = 'a' + rand() % 26;
    name[i] = ch;
  }
  name[len - 1] = '\0';
  sleep(1);
}