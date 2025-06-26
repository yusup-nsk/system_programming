#include <stdio.h>

#define BYTE_NUMBER 3
#define BYTE_INDEX (BYTE_NUMBER - 1)

int IsLittleEndian(void);
void OutputBytesOfInt(int const *const num_ptr);

int main() {
  int num;
  unsigned char insertion;
  if (scanf("%d%hhu", &num, &insertion) == 2 && (num > 0)) {
    printf("Bytes of integer before insertion: ");
    OutputBytesOfInt(&num);
    char *ptr = (char *)(&num);
    if (IsLittleEndian()) {
      ptr += BYTE_INDEX;
    } else {
      ptr += 3 - BYTE_INDEX;
    }
    *ptr = insertion;
    printf("Bytes of integer after insertion:  ");
    OutputBytesOfInt(&num);
  }
  return 0;
}

int IsLittleEndian(void) {
  int one = 1;
  char ch = *((char *)(&one));
  return ch == 1;
}

void OutputBytesOfInt(int const *const num_ptr) {
  unsigned char *char_ptr = (unsigned char *)num_ptr;
  if (IsLittleEndian()) {
    for (int i = 3; i >= 0; --i) {
      printf("%3hhu.", char_ptr[i]);
    }
  } else {
    for (int i = 0; i < 4; ++i) {
      printf("%3hhu.", char_ptr[i]);
    }
  }
  putchar('\n');
}