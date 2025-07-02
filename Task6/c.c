

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  int *ptr;
  printf("before malloc pointer is :%p \n", ptr);
  printf("before malloc valu is :%d \n", *ptr);
  ptr = malloc(sizeof(int));
  printf("after malloc pointer is %p \n", ptr);
  printf("after malloc valu is :%d \n", *ptr);
  int jig = 32;
  *ptr = jig;
  printf("after assignment valu is : %d\n", *ptr);
  free(ptr);
  printf("after free %p \n",
         ptr);  // after free pointer holds sane address then
  printf("after fee is %d\n", *ptr);  // why it coudnt print that???
  return 0;
}