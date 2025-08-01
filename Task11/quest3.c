
#include <assert.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define START_VALUE 5000
#define LOADER_VALUE 300
#define CONSUMER_NEEDS 5500
#define N_SHOPS 5
#define N_CONSUMERS 3
#define SLEEP_CONSUMER 2
#define SLEEP_LOADER 1
// #define NO_COLOR printf("\033[0m")
// #define SET_COLOR_32 printf("\033[1;32m")
// #define SET_COLOR_33 printf("\033[1;33m")
// #define SET_COLOR_34 printf("\033[1;34m")



pthread_mutex_t mute_shop[N_SHOPS] = PTHREAD_MUTEX_INITIALIZER;
int shop[N_SHOPS];
int needs[N_CONSUMERS];


void fill_shops_in_start(int shop[N_SHOPS]);
void fill_consumers_needs(int needs[N_CONSUMERS]);
void show(int shop[N_SHOPS], int needs[N_CONSUMERS]);
void make_next_shop_index(int consumer_index, int* shop_index);
void set_color(unsigned color_num);
void* consumers_behavior(void* arg);


int main() {
  srand(time(NULL));
  int* status;
  //   int shop[N_SHOPS];
  //   int needs[N_CONSUMERS];
  fill_shops_in_start(shop);
  fill_consumers_needs(needs);
  putchar('\n');
  int var[N_CONSUMERS];

   pthread_t consumer[N_CONSUMERS];
//    pthread_t loader;

  for (int i = 0; i < N_CONSUMERS; ++i) {
    var[i] = i;
    pthread_create(&consumer[i], NULL, consumers_behavior, (int**)&var[i]);
  }

  for (int i = 0; i < N_CONSUMERS; ++i) {
    pthread_join(consumer[i], (void**)&status);
  }

  // show(shop, needs);

  int is_any_needs = 0;
  for(int j=0; j<N_CONSUMERS; j++){
    is_any_needs = is_any_needs || needs[j];
  }
  if (!is_any_needs){
    printf("\nEACH NEEDS SATISFIED!!!\n");
  }


  exit(EXIT_SUCCESS);
}

void fill_shops_in_start(int shop[N_SHOPS]) {
  srand(time(NULL));
  for (int i = 0; i < N_SHOPS; ++i) {
    shop[i] = START_VALUE - (START_VALUE * 0.3) +
              (START_VALUE * 0.3 * 2 * rand() / RAND_MAX);
  }
  for (int i = 0; i < N_SHOPS; ++i) {
    printf("shop #%d: has %d products\n", i, shop[i]);
  }
}

void fill_consumers_needs(int needs[N_CONSUMERS]) {
  srand(time(NULL));
  for (int i = 0; i < N_CONSUMERS; ++i) {
    needs[i] = CONSUMER_NEEDS - (CONSUMER_NEEDS * 0.3) +
               (CONSUMER_NEEDS * 0.3 * 2 * rand() / RAND_MAX);
    printf("Consumer #%d: at start needs %d products\n", i, needs[i]);
  }
}

void show(int shop[N_SHOPS], int needs[N_CONSUMERS]) {
  putchar('\n');
  putchar('\n');
  putchar('\n');
  printf("LOADER is ...\n");
  for (int i = 0; i < N_SHOPS; ++i) {
    printf("shop %d:%d      ", i + 1, shop[i]);
  }
  putchar('\n');
  putchar('\n');
  for (int i = 0; i < N_CONSUMERS; ++i) {
    printf("  %d:::%d", i + 1, needs[i]);
  }
  putchar('\n');
  putchar('\n');
}

void make_next_shop_index(int consumer_index, int* shop_index) {
  if (consumer_index) {
    *shop_index = (*shop_index + 1) % N_SHOPS;
  } else {
    *shop_index = (*shop_index - 1 + N_SHOPS) % N_SHOPS;
  }
}


void set_color(unsigned color_num){
    char str[20];
    sprintf(str, "\033[%dm", color_num) ;
    printf("%s", str);
}


void* consumers_behavior(void* arg) {
  int cons_index = *(int*)arg;
  set_color(cons_index +32);
  printf("Consumer #%d starts, needs %d\n", cons_index, needs[cons_index]);
//   pthread_t this_tid = pthread_self();
  int shop_index = (cons_index * N_SHOPS / 2) % N_SHOPS;
  int did_purchase = 0;
//   int visited_shops = 0;
  int has_needs = 1;
  for (; has_needs; make_next_shop_index(cons_index, &shop_index)) {
    pthread_mutex_lock(&mute_shop[shop_index]);
    printf("Consumer #%d come to shop #%d. Needs = %d\n", cons_index,
           shop_index, needs[cons_index]);
    if (shop[shop_index]) {
      if (shop[shop_index] >= needs[cons_index]) {
        shop[shop_index] -= needs[cons_index];
        needs[cons_index] = 0;
        has_needs = 0;
        printf("Consumer #%d: Satisfied the needs!!! \n", cons_index);
      } else {
        needs[cons_index] -= shop[shop_index];
        shop[shop_index] = 0;
        did_purchase = 1;
        printf("Consumer #%d: Did purchase....\n", cons_index);
      }
    }
    pthread_mutex_unlock(&mute_shop[shop_index]);
    if (did_purchase && has_needs) {
      printf(
          "Consumer #%d: After shopping in shop #%d, I'm going to sleep with "
          "needs = %d\n",
          cons_index, shop_index, needs[cons_index]);
      sleep(SLEEP_CONSUMER);
    }
  }
  set_color(0);
  return needs;
}
