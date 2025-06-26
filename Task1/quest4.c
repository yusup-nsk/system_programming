#include <stdio.h>

#include "output_func.c"

#define BYTE_NUMBER 3
#define BYTE_INDEX (BYTE_NUMBER - 1)

int main() {
  int num;
  unsigned char insertion;
  if (scanf("%d%hhu", &num, &insertion) == 2 && (num > 0)) {
    OutputBitsOfInt(num);
    putchar('\n');
    int mask = 0xFF << (BYTE_INDEX * 8);
    mask = ~mask;
    num = num & mask;
    mask = insertion << (BYTE_INDEX * 8);
    num = num | mask;
    OutputBitsOfInt(num);
    putchar('\n');
  }
  return 0;
}