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

int CountUnitBitsOfUnsignedChar(unsigned char uch) {
  unsigned char mask_uc;
  int sum = 0;
  for (int i = 7; i >= 0; --i) {
    mask_uc = 1 << i;
    sum += (uch & mask_uc) >> i;
  }
  return sum;
}

int CountUnitBitsOfInt(int num) {
  int sum = 0;
  for (int byte = 3; byte >= 0; --byte) {
    unsigned int uint = (num & (0b11111111 << 8 * byte)) >> 8 * byte;
    unsigned char uch = uint & 0b11111111;
    sum += CountUnitBitsOfUnsignedChar(uch);
  }
  return sum;
}