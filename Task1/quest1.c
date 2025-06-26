#include <stdio.h>
#include "output_func.c"

int main() {
  int num;
  if (scanf("%d", &num) == 1 && (num > 0)) {
    output_bits_of_int(num);
    putchar('\n');
  }
  return 0;
}