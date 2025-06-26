#include <stdio.h>

#include "functions_for_output_matrix.c"

int main() {
  int n;
  if (scanf("%d", &n) != 1 || n < 1) return 0;

  if (n == 1) {
    printf("1\n");
    return 0;
  }

  int matrix[MAX_SIZE_OF_MATRIX][MAX_SIZE_OF_MATRIX];
  int value = 1;
  int i = 0, j = 0;
  int min_row = 0, max_row = n - 1;
  int min_col = 0, max_column = n - 1;
  int repeat = 1;

  for (; repeat && j < max_column; ++j) {
    matrix[i][j] = value++;
    if (value >= n * n) {
      repeat = 0;
    }
  }
  ++min_row;
  for (; repeat && i < max_row; ++i) {
    matrix[i][j] = value++;
    if (value >= n * n) {
      repeat = 0;
    }
  }
  --max_column;

  while (repeat) {
    for (; repeat && j > min_col; --j) {
      matrix[i][j] = value++;
      if (value >= n * n) {
        repeat = 0;
      }
    }
    --max_row;
    for (; repeat && i > min_row; --i) {
      matrix[i][j] = value++;
      if (value >= n * n) {
        repeat = 0;
      }
    }
    ++min_col;
    for (; repeat && j < max_column; ++j) {
      matrix[i][j] = value++;
      if (value >= n * n) {
        repeat = 0;
      }
    }
    ++min_row;
    for (; repeat && i < max_row; ++i) {
      matrix[i][j] = value++;
      if (value >= n * n) {
        repeat = 0;
      }
    }
    --max_column;
  }

  if (n % 2)
    matrix[n / 2][n / 2] = value;
  else
    matrix[n / 2][n / 2 - 1] = value;

  OutputMatrixOfInteger(matrix, n, CountDigits(n * n));
  return 0;
}