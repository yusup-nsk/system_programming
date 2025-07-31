
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define START_VALUE 500
#define LOADER_VALUE 300
#define CONSUMER_NEEDS 5500
#define N_SHOPS 5
#define N_CONSUMERS 3
#define SLEEP_CONSUMER 2
#define SLEEP_LOADER 1

void fill_shops_in_start(int shop[N_SHOPS]);
void fill_consumer_needs(int needs[N_CONSUMERS]);
void show(int shop[N_SHOPS], int needs[N_CONSUMERS]) {
  putchar('\n');
  putchar('\n');
  putchar('\n');
  printf("LOADER is ...\n");
  for (int i = 0; i < N_SHOPS; ++i) {
    printf("shop %d:%d      ", i + 1, shop[i]);
  }
  putchar('\n'); putchar('\n');
  for (int i = 0; i < N_CONSUMERS; ++i) {
    printf("  %d:::%d", i + 1, needs[i]);
  }
  putchar('\n'); putchar('\n');
}

int main() {
  srand(time(NULL));
  int shop[N_SHOPS];
  int needs[N_CONSUMERS];
  fill_shops_in_start(shop);
  fill_consumer_needs(needs);
  
  pid_t pid0 = getpid();
  pid_t pid = fork();
  pid_t consumer[N_CONSUMERS], loader;
  consumer[0] = pid;
  if (pid) {
    pid = fork();
    consumer[1] = pid;
  }
  if (getpid() == pid0) {
    pid = fork();
    consumer[2] = pid;
  }
  if (getpid() == pid0) {
    pid = fork();
    loader = pid;
  }

  if (getpid() != pid0) {  // у всех потоков один родитель
    assert(getppid() == pid0);
  }

  show(shop, needs);

  sleep(20);

  exit(EXIT_SUCCESS);
}

void fill_shops_in_start(int shop[N_SHOPS]) {
  srand(time(NULL));
  for (int i = 0; i < N_SHOPS; ++i) {
    shop[i] = START_VALUE - (START_VALUE * 0.3) +
              (START_VALUE * 0.3 * 2 * rand() / RAND_MAX);
  }
//   for (int i = 0; i < N_SHOPS; ++i) {
//     printf("shop #%d: %d\n", i, shop[i]);
//   }
}

void fill_consumer_needs(int needs[N_CONSUMERS]) {
  srand(time(NULL));
  for (int i = 0; i < N_CONSUMERS; ++i) {
    needs[i] = CONSUMER_NEEDS - (CONSUMER_NEEDS * 0.3) +
               (CONSUMER_NEEDS * 0.3 * 2 * rand() / RAND_MAX);
    // printf("needs[%d]=%d\n", i, needs[i]);
  }
}
