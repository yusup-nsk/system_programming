
#include <pthread.h>
#include <stdio.h>

#define N 10

void* worker(void* arg) {
  int* i = (int*)arg;
 printf("pthread number is %d\n", *i);
  return NULL;
}

int main() {
  int i;
 int v[N];
  int* status;
  pthread_t tid[N];

  for (i = 0; i < N; i++) {
    v[i] = i;
    pthread_create(&tid[i], NULL, worker, (void*)&v[i]);
  }

  for (i = 0; i < N; i++) {
    pthread_join(tid[i], (void**)&status);
  }
  return 0;
}