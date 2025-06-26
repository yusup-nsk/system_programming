#include <stdio.h>

#define MAX_ARRAY_SIZE 1024

int main() {
  int arr[MAX_ARRAY_SIZE];
  int size = 0;
  char ch;
  while (scanf("%d%c", arr + size++, &ch) == 2 && ch != '\n')
    ;
  for (int i = 0; i < size / 2; ++i) {
    int tmp = arr[i];
    arr[i] = arr[size - 1 - i];
    arr[size - 1 - i] = tmp;
  }
  for (int i = 0; i < size; ++i) {
    printf("%d ", arr[i]);
  }
  putchar('\n');
  return 0;
}

/*
числа массива вводятся пользователем через пробел
после последнего числа - нажать <Enter>
затем числа переставляются в обратном порядке
и выводятся на экран
*/
