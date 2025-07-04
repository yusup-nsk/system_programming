#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_ABONENTS 100
#define DATA_LENGTH 10 //длина строки с данными вместе с '\0'
#define MAX_LENGTH 256
#define SLEEP_SECONDS 2 // пауза в секундах

struct abonent {
  char name[DATA_LENGTH];
  char second_name[DATA_LENGTH];
  char tel[DATA_LENGTH];
};

int InputData(char *); /* отсеивает пробельные символы и
считывает первые ( DATA_LENGTH-1 ) символы   в введенном слове
*/
void OutputAbonentData(struct abonent abonent) {
  printf("Имя: %-*s  Фамилия: %-*s  Телефон: %-*s\n", DATA_LENGTH, abonent.name,
         DATA_LENGTH, abonent.second_name, DATA_LENGTH, abonent.tel);
}

int main() {
  struct abonent phone_book[MAX_ABONENTS];
  int amount_of_abonents = 0;
  const char title[] = "\n\t\tАбонентский справочник\n\n";
  const char menu[] =
      "1) Добавить абонента\n2) Удалить абонента\n3) Поиск абонентов по "
      "имени\n4) Вывод всех записей\n5) Выход\n\n";
  char data[DATA_LENGTH];
  int repeat = 1;
  int menu_choice = 0;
  printf("\n\n\n\n\n\n\n\n");

  while (repeat) {
    printf("\n%s%s", title, menu);
    printf("Введите пункт меню: ");
    if (InputData(data) == 0 || data[0] == '\n' || data[0] == '\0')
      continue;
    if (sscanf(data, "%d", &menu_choice) == 1 && menu_choice > 0 &&
        menu_choice < 6) {
      switch (menu_choice) {
      case 1: // добавление абонента
        if (amount_of_abonents == MAX_ABONENTS) {
          printf("Справочник переполнен, невозможно добавить абонента\n");
        } else {
          printf("Введите имя абонента:\n");
          while (InputData(phone_book[amount_of_abonents].name) == 0)
            ;
          printf("Введите фамилию абонента:\n");
          while (InputData(phone_book[amount_of_abonents].second_name) == 0)
            ;
          printf("Введите телефон абонента:\n");
          while (InputData(phone_book[amount_of_abonents].tel) == 0)
            ;
          printf("Добавлен абонент:\n");
          OutputAbonentData(phone_book[amount_of_abonents]);
          amount_of_abonents++;
        }
        sleep(SLEEP_SECONDS);
        break;
      case 2: // удаление: на место удаляемой записи копируется последняя по
              // номеру  запись, место в памяти, занимавшая последняя запись
              // обнуляется, количество записей уменьшается на 1
        if (amount_of_abonents == 0) {
          printf("Справочник пуст, нечего удалять\n");
          sleep(SLEEP_SECONDS);
          break;
        }
        printf("Введите порядковый номер записи в справочнике, который хотите "
               "удалить:\n");
        InputData(data);
        int index;
        int scanned = sscanf(data, "%d", &index);
        --index;
        if (scanned != 1) {
          printf("Это не номер\n");
        } else if (index < 0 || index >= amount_of_abonents) {
          { printf("Нет записи с таким номером\n"); }
        } else {
          strncpy(phone_book[index].name,
                  phone_book[amount_of_abonents - 1].name, DATA_LENGTH);
          strncpy(phone_book[index].second_name,
                  phone_book[amount_of_abonents - 1].second_name, DATA_LENGTH);
          strncpy(phone_book[index].tel, phone_book[amount_of_abonents - 1].tel,
                  DATA_LENGTH);
          for (int i = 0; i < DATA_LENGTH; ++i) {
            phone_book[amount_of_abonents - 1].name[i] = 0;
            phone_book[amount_of_abonents - 1].second_name[i] = 0;
            phone_book[amount_of_abonents - 1].tel[i] = 0;
          }
          --amount_of_abonents;
          printf("Запись удалена\n");
        }
        sleep(SLEEP_SECONDS);
        break;
      case 3: // поиск по имени
        if (amount_of_abonents == 0) {
          printf("Справочник пуст, нечего искать\n");
          sleep(SLEEP_SECONDS);
          break;
        }
        do {
          printf("Введите имя для поиска:\n");
          InputData(data);
        } while ( '\0' == data[0]);
        int found = 0;
        for (int i = 0; i < amount_of_abonents; ++i) {
          if (strncmp(data, phone_book[i].name, DATA_LENGTH) == 0) {
            printf("%3d. ", i + 1);
            OutputAbonentData(phone_book[i]);
            found++;
          }
        }
        if (found) {
          printf("Найдено %d записей абонентов с именем \"%s\"\n", found, data);
        }

        else {
          printf("Не найдено ни одной записи с именем \"%s\"\n", data);
        }
        sleep(SLEEP_SECONDS);
        break;
      case 4: // вывод списка абонентов
        if (amount_of_abonents == 0) {
          printf("Справочник пуст, нечего выводить\n");
          sleep(SLEEP_SECONDS);
          break;
        }
        printf("\nСписок всех абонентов: \n");
        for (int i = 0; i < amount_of_abonents; ++i) {
          printf("%3d. ", i + 1);
          OutputAbonentData(phone_book[i]);
        }
        printf("\n");
        sleep(2 * SLEEP_SECONDS);
        break;
      case 5: // выход из программы
        repeat = 0;
        break;
      }
    } else {
      printf("Неправильно введен пункт меню\n");
      sleep(SLEEP_SECONDS);
    }
  } // while
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
