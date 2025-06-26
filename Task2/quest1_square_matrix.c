#include <stdio.h>

#include "functions_for_output_matrix.c"

int main() {
  int n;
  if (scanf("%d", &n) == 1 && n > 0) {
    int value = 0;
    int matrix[MAX_SIZE_OF_MATRIX][MAX_SIZE_OF_MATRIX];
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        matrix[i][j] = ++value;
      }
    }
    OutputMatrixOfInteger(matrix, n, CountDigits(n * n));
  }
  return 0;
}
