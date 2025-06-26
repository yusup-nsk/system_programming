#include <stdio.h>

#include "functions_for_output_matrix.c"

int main() {
  int n;
  if (scanf("%d", &n) != 1 || n < 1) return 0;

  int matrix[MAX_SIZE_OF_MATRIX][MAX_SIZE_OF_MATRIX];
  for (int i = 0; i < n; ++i) {
    for (int j = n - i - 1; j < n; ++j) matrix[i][j] = 1;
  }
  for (int i = 0; i < n - 1; ++i) {
    for (int j = 0; j < n - i - 1; ++j) matrix[i][j] = 0;
  }
  OutputMatrixOfInteger(matrix, n, 1);
  return 0;
}