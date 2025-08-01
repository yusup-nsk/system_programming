#include <pthread.h>
#include <stdio.h>

#define N 8

long a = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void *worker(void *arg) {
  int i;
  for (i = 0; i < 100000000; i++) {
    pthread_mutex_lock(&mut);
    a++;
    pthread_mutex_unlock(&mut);
  }
  return NULL;
}

int main() {
  int i;
  int *status;
  pthread_t tid[N];

  for (i = 0; i < N; i++) {
    pthread_create(&tid[i], NULL, worker, NULL);
  }

  for (i = 0; i < N; i++) {
    pthread_join(tid[i], (void **)&status);
  }

  printf("a = %ld\n", a);
  return 0;
}