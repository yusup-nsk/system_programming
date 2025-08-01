#include <pthread.h>
#include <stdio.h>

#define N 8
#define ITERATIONS 100000000

long a = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void *worker(void *arg) {
  int i;
  if (NULL == arg) {
    for (i = 0; i < ITERATIONS; i++) {
      a++;
    }
  } else {
    for (i = 0; i < ITERATIONS; i++) {
      pthread_mutex_lock(&mut);
      a++;
      pthread_mutex_unlock(&mut);
    }
  }
  return NULL;
}

int main() {
  long a_with_mutex;
  int i;
  int *status;
  pthread_t tid[N];

  for (i = 0; i < N; i++) {
    pthread_create(&tid[i], NULL, worker, (void *)&i);
  }
  for (i = 0; i < N; i++) {
    pthread_join(tid[i], (void **)&status);
  }

  a_with_mutex = a;
  a = 0;

  for (i = 0; i < N; i++) {
    pthread_create(&tid[i], NULL, worker, NULL);
  }
  for (i = 0; i < N; i++) {
    pthread_join(tid[i], (void **)&status);
  }

  printf("Result of increments with mutex = %ld,  without mutex = %ld\n",
         a_with_mutex, a);
  printf("Error rate is %.2lf%%\n",
         (100.0 * (a_with_mutex - a)) / a_with_mutex);
  return 0;
}
