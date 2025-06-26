#include <stdio.h>

#include "output_func.c"

int CountUnitBitsOfUnsignedChar(unsigned char);
int CountUnitBitsOfInt(int);

int main() {
  int num;
  if (scanf("%d", &num) == 1) {
    OutputBitsOfInt(num);
    putchar('\n');
    printf("Number of unit bits == %d\n", CountUnitBitsOfInt(num));
  }
  return 0;
}

int CountUnitBitsOfUnsignedChar(unsigned char unsigned_char) {
  unsigned char mask;
  int sum = 0;
  for (int i = 7; i >= 0; --i) {
    mask = 1 << i;
    sum += (unsigned_char & mask) >> i;
  }
  return sum;
}

int CountUnitBitsOfInt(int num) {
  int sum = 0;
  for (int byte = 3; byte >= 0; --byte) {
    unsigned int unsigned_int = (num & (0xFF << 8 * byte))  >> 8 * byte;
    unsigned char unsigned_char = unsigned_int & 0xFF;
    sum += CountUnitBitsOfUnsignedChar(unsigned_char);
  }
  return sum;
}