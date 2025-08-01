#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define START_VALUE 500
#define LOADER_VALUE 300
#define CONSUMER_NEEDS 5500
#define N_SHOPS 5
#define N_CONSUMERS 3
#define SLEEP_CONSUMER 2
#define SLEEP_LOADER 1

#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define VIOLET "\033[1;35m"

pthread_mutex_t mute_shop[N_SHOPS] = PTHREAD_MUTEX_INITIALIZER;
int shop[N_SHOPS];
int needs[N_CONSUMERS];
const char colour[N_CONSUMERS][12] = {GREEN, YELLOW, BLUE};
int g_do_loading = N_CONSUMERS;

int fill_shops_in_start(int shop[N_SHOPS]);
int fill_consumers_needs(int needs[N_CONSUMERS]);
void make_next_shop_index(int consumer_index, int* shop_index);
void* consumers_behavior(void* arg);
void* loader_behavior(void* arg);
void output_statistics(int s_begin, int needs, int s_end, int needs_at_end,
                       int loaded);

int main() {
  int* status;
  int var[N_CONSUMERS];
  pthread_t consumer[N_CONSUMERS];
  pthread_t loader;
  int all_products_at_beginnig = fill_shops_in_start(shop);
  int all_needs_at_beginning = fill_consumers_needs(needs);
  int all_products_at_end = 0;
  int all_needs_at_end = 0;
  int total_loaded_products = 0;
  printf("Total products in shops: %d\nTotal needs: %d\n\n",
         all_products_at_beginnig, all_needs_at_beginning);

  for (int i = 0; i < N_CONSUMERS; ++i) {
    var[i] = i;
    pthread_create(&consumer[i], NULL, consumers_behavior, (int**)&var[i]);
  }
  pthread_create(&loader, NULL, loader_behavior, &total_loaded_products);
  for (int i = 0; i < N_CONSUMERS; ++i) {
    pthread_join(consumer[i], (void**)&status);
    printf("%sConsumer \033[5m#%i finished his thread\033[0m\n", colour[i], i);
  }
  pthread_join(loader, (void**)&status);
  printf("%sLoader finished work\n", VIOLET);

  for (int i = 0; i < N_SHOPS; i++) {
    all_products_at_end += shop[i];
  }
  for (int j = 0; j < N_CONSUMERS; j++) {
    all_needs_at_end += needs[j];
  }
  if (0 == all_needs_at_end) {
    printf("\033[1;36m\nEACH NEEDS SUCCESSFULLY SATISFIED!!!\n");
  } else {
    printf("\033[5mSOMETHING GONE WRONG!\n");
  }

  output_statistics(all_products_at_beginnig, all_needs_at_beginning,
                    all_products_at_end, all_needs_at_end,
                    total_loaded_products);
  exit(EXIT_SUCCESS);
}

int fill_shops_in_start(int shop[N_SHOPS]) {
  srand(time(NULL));
  int sum = 0;
  for (int i = 0; i < N_SHOPS; ++i) {
    shop[i] = START_VALUE - (START_VALUE * 0.3) +
              (START_VALUE * 0.3 * 2 * rand() / RAND_MAX);
    printf("shop #%d: has %d products\n", i, shop[i]);
    sum += shop[i];
  }
  return sum;
}

int fill_consumers_needs(int needs[N_CONSUMERS]) {
  srand(time(NULL));
  int sum = 0;
  for (int i = 0; i < N_CONSUMERS; ++i) {
    needs[i] = CONSUMER_NEEDS - (CONSUMER_NEEDS * 0.3) +
               (CONSUMER_NEEDS * 0.3 * 2 * rand() / RAND_MAX);
    printf("Consumer's needs[%d]=%d\n", i, needs[i]);
    sum += needs[i];
  }
  return sum;
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
  printf("\033[0mConsumer #%d starts, needs %d\n", cons_index,
         needs[cons_index]);
  int shop_index = (cons_index * N_SHOPS / 2) % N_SHOPS;
  int has_needs = 1;
  int visited_shops = 0;
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
        printf("\n\t\t\t%sConsumer #%d: Satisfied the needs!!! \033[0m\n\n",
               color, cons_index);
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
    if (!did_purchase && has_needs && visited_shops % N_SHOPS == 0) {
      printf(
          "%sConsumer #%d: I searched ALL shops and found NO product, I'm "
          "tired and going to sleep with "
          "needs = %d\n",
          color, cons_index, *this_needs);
    }
    if ((did_purchase && has_needs) || visited_shops % N_SHOPS == 0) {
      sleep(SLEEP_CONSUMER);
    }
  }
  return NULL;
}

void* loader_behavior(void* arg) {
  int* total_loaded = (int*)arg;
  int shop_index = 0;
  while (g_do_loading) {
    pthread_mutex_lock(&mute_shop[shop_index]);
    shop[shop_index] += LOADER_VALUE;
    *total_loaded += LOADER_VALUE;
    printf("%sLoader brings %d products to shop #%d and goes to sleep\n",
           VIOLET, LOADER_VALUE, shop_index);
    pthread_mutex_unlock(&mute_shop[shop_index]);
    shop_index = (shop_index + 1) % N_SHOPS;
    sleep(SLEEP_LOADER);
  }
  return NULL;
}

void output_statistics(int all_products_at_beginnig, int all_needs_at_beginning,
                       int all_products_at_end, int all_needs_at_end,
                       int total_loaded_products) {
  printf(
      "\n\033[0mStatistiсs at the beginning: \nTotal %d products in shops;  "
      "Total "
      "consumer's needs were  %d products\n",
      all_products_at_beginnig, all_needs_at_beginning);
  printf(
      "\nStatistiсs at the end: \nTotal %d products in shops;  Total "
      "consumer's needs are  %d products\n",
      all_products_at_end, all_needs_at_end);
  printf("Total loaded %d products\n\n", total_loaded_products);
  int disbalance = all_products_at_beginnig + total_loaded_products -
                   all_products_at_end - all_needs_at_beginning;
  printf("Disbalance ( S_begin + Loaded - S_end - Needs ) is %d.\n",
         disbalance);
}