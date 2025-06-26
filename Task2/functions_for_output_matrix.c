#define MAX_SIZE_OF_MATRIX 100
int CountDigits(int num) {
  int count_digits = 0;
  while (num) {
    num /= 10;
    ++count_digits;
  }
  return count_digits;
}

void OutputMatrixOfInteger(int matrix[MAX_SIZE_OF_MATRIX][MAX_SIZE_OF_MATRIX],
                           int size, int length_of_element) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      printf("%*d ", length_of_element, matrix[i][j]);
    }
    putchar('\n');
  }
}