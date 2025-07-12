#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define DATA_LENGTH 12  //длина строки с данными вместе с '\0'
#define MAX_LENGTH 256
#define SLEEP_SECONDS 2  // пауза в секундах

int InputData(char *);
extern void add(int, int);
extern void div(int, int);
extern void mul(int, int);
extern void sub(int, int);

int main() {
  const char title[] = "\n\t\tЦелочисленный калькулятор\n\n";
  const char menu[] =
      "1) Сложение\n"
      "2) Вычитание\n"
      "3) Умножение\n"
      "4) Деление\n"
      "5) Выход\n";

  char data[DATA_LENGTH];
  int menu_choice;
  printf("\n\n\n\n\n");
  while (1) {
    printf("\n%s%s", title, menu);
    printf("Введите пункт меню: ");
    if (0 == InputData(data)) continue;
    if (sscanf(data, "%d", &menu_choice) == 1 && menu_choice > 0 &&
        menu_choice < 6) {
      int number_1, number_2;
      if (menu_choice == 5) {
        break;
      }
      do {
        printf("Введите первое целое число:\n");
      } while (0 == InputData(data) || 1 != sscanf(data, "%d", &number_1));
      do {
        printf("Введите второе целое число:\n");
      } while (0 == InputData(data) || 1 != sscanf(data, "%d", &number_2));
      switch (menu_choice) {
        case 1:
          add(number_1, number_2);
          sleep(SLEEP_SECONDS);
          break;
        case 2:
          sub(number_1, number_2);
          sleep(SLEEP_SECONDS);
          break;
        case 3:
          mul(number_1, number_2);
          sleep(SLEEP_SECONDS);
          break;
        case 4:
          div(number_1, number_2);
          sleep(SLEEP_SECONDS);
          break;
      }
    } else {
      printf("Неправильно введен пункт меню\n");
      sleep(SLEEP_SECONDS);
    }
  }  // while
  return 0;
}

int InputData(char *data) {
  data[0] = '\0';
  char str[MAX_LENGTH];
  fgets(str, MAX_LENGTH, stdin);
  char format[8];
  sprintf(format, " %%%ds", DATA_LENGTH - 1);
  sscanf(str, format, data);
  return (strlen(data));
}