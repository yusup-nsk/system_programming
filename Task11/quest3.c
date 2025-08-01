
#include <assert.h>
#include <pthread.h>
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

#define WHITE "\033[0m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define VIOLET "\033[1;35m"

pthread_mutex_t mute_shop[N_SHOPS] = PTHREAD_MUTEX_INITIALIZER;
int shop[N_SHOPS];
int needs[N_CONSUMERS];
const char colour[N_CONSUMERS][12] = {GREEN, YELLOW, BLUE};
int g_do_loading = N_CONSUMERS;

void fill_shops_in_start(int shop[N_SHOPS]);
void fill_consumers_needs(int needs[N_CONSUMERS]);
void make_next_shop_index(int consumer_index, int* shop_index);
void* consumers_behavior(void* arg);
void* loader_behavior(void* arg);

int main() {
  int* status;
  fill_shops_in_start(shop);
  fill_consumers_needs(needs);
  putchar('\n');
  int var[N_CONSUMERS];
  pthread_t consumer[N_CONSUMERS];
  pthread_t loader;

  for (int i = 0; i < N_CONSUMERS; ++i) {
    var[i] = i;
    pthread_create(&consumer[i], NULL, consumers_behavior, (int**)&var[i]);
  }
  // int repeat_loading = 1;
  // pthread_create(&loader, NULL, loader_behavior, (void*)&repeat_loading);
  pthread_create(&loader, NULL, loader_behavior, NULL);
  for (int i = 0; i < N_CONSUMERS; ++i) {
    pthread_join(consumer[i], (void**)&status);
    printf("%sConsumer \033[5m#%i finished his thread%s\n", colour[i], i,
           WHITE);
  }
  // g_repeat_loading = 0;
  pthread_join(loader, (void**)&status);
  printf("%sLoader finished work\n", VIOLET);
  int is_any_needs = 0;
  for (int j = 0; j < N_CONSUMERS; j++) {
    is_any_needs = is_any_needs || needs[j];
  }
  if (!is_any_needs) {
    printf("\033[1;36m\nEACH NEEDS SATISFIED!!!\n");
  } else
    printf("\033[5mSOMETHING GONE WRONG!\n");
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
  }
}

void make_next_shop_index(int consumer_index, int* shop_index) {
  if (consumer_index) {
    *shop_index = (*shop_index + 1) % N_SHOPS;
  } else {
    *shop_index = (*shop_index - 1 + N_SHOPS) % N_SHOPS;
  }
}

void* consumers_behavior(void* arg) {
  int cons_index = *(int*)arg;
  printf("%sConsumer #%d starts, needs %d\n", WHITE, cons_index,
         needs[cons_index]);
  int shop_index = (cons_index * N_SHOPS / 2) % N_SHOPS;
  // int did_purchase = 0;
  int has_needs = 1;
  int visited_shops =  0;
  int* const this_needs = &needs[cons_index];
  const char* const color = (const char* const)&colour[cons_index];
  for (; has_needs; make_next_shop_index(cons_index, &shop_index)) {
    int did_purchase = 0;
    visited_shops++;
    int* const products_in_this_shop = &shop[shop_index];
    pthread_mutex_lock(&mute_shop[shop_index]);
    printf("%sConsumer #%d come to shop #%d. Needs = %d\n", color, cons_index,
           shop_index, *this_needs);
    if (*products_in_this_shop) {
      if (*products_in_this_shop >= *this_needs) {
        *products_in_this_shop -= *this_needs;
        *this_needs = 0;
        has_needs = 0;
        --g_do_loading;
        printf("\n\t\t\t%sConsumer #%d: Satisfied the needs!!! \n\n", color,
               cons_index);
      } else {
        *this_needs -= *products_in_this_shop;
        *products_in_this_shop = 0;
        did_purchase = 1;
        printf("%sConsumer #%d: Did purchase....\n", color, cons_index);
      }
    } else {
      printf(
          "%sConsumer #%d: No products in shop #%d, "
          "I still have needs = %d\n",
          color, cons_index, shop_index, *this_needs);      
    }
    if (did_purchase && has_needs) {
      printf(
          "%sConsumer #%d: After shopping in shop #%d, I'm going to sleep with "
          "needs = %d\n",
          color, cons_index, shop_index, *this_needs);
    }
    pthread_mutex_unlock(&mute_shop[shop_index]);
        if (!did_purchase && has_needs && visited_shops % N_SHOPS ==0) {
      printf(
          "%sConsumer #%d: I searched ALL shops and found NO product, I'm tired and going to sleep with "
          "needs = %d\n",
          color, cons_index,  *this_needs);
    }
    if ((did_purchase && has_needs) || visited_shops % N_SHOPS ==0) {
      sleep(SLEEP_CONSUMER);
    }
  }
  return NULL;
}

void* loader_behavior(void* arg) {
  // int* do_loading = (int*)arg;
  int shop_index = 0;
  if (NULL == arg)
    while (g_do_loading) {
      pthread_mutex_lock(&mute_shop[shop_index]);
      shop[shop_index] += LOADER_VALUE;
      printf("%sLoader brings %d products to shop #%d and goes to sleep\n",
             VIOLET, LOADER_VALUE, shop_index);
      pthread_mutex_unlock(&mute_shop[shop_index]);
      shop_index = (shop_index + 1) % N_SHOPS;
      sleep(SLEEP_LOADER);
    }
  return NULL;
}