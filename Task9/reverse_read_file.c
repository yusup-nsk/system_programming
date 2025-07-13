#include <stdio.h>
#include <stdlib.h>

int main() {
  const char str[] = "String from file";
  FILE *file = fopen("output.txt", "w+");
  if (file) {
    fprintf(file, "%s", str);
    fseek(file, 0, SEEK_END);
    int size_of_file = ftell(file);
    for (int i = size_of_file - 1; i >= 0; --i) {
      fseek(file, i, SEEK_SET);
      printf("%c", fgetc(file));
    }
    printf("\n");
    fclose(file);
  }
  return 0;
}