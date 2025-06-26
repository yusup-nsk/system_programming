#include <stdio.h>

int main() {
  int arr[10];
  for (int i = 0; i < 10; ++i) arr[i] = i + 1;
  int *finish = arr + 10;
  for (int *current = arr; current != finish; current++) {
    printf("%d ", *current);
  }
  putchar('\n');
  return 0;
}