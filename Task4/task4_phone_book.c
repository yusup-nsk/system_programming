#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_ABONENTS 8 // TODO 100
#define DATA_LENGTH 12
#define MAX_LENGTH 256
#define SLEEP_SECONDS 2

struct abonent {
  char name[DATA_LENGTH];
  char second_name[DATA_LENGTH];
  char tel[DATA_LENGTH];
};

void InputData(char *); /* отсеивает пробельные символы и 
считывает первые ( DATA_LENGTH-1 ) символы   в введенном слове
*/
void OutputAbonentData(struct abonent abonent) {
  printf("Имя: %-*s   Фамилия: %-*s   Телефон: %-*s\n", DATA_LENGTH,
         abonent.name, DATA_LENGTH, abonent.second_name, DATA_LENGTH,
         abonent.tel);
}

int main() {
  struct abonent phone_book[MAX_ABONENTS] = { 
      {"aaaaa", "AAAAAAAA", "0000000"},// TODO DELETE
      {"bbbbbbbbb", "BBBBBBBB", "1111111111"},
      {"cccc", "CCCCCCC", "2222222222"},
      {"ddddddddddd", "DDDDDDDD", "3333333"},
      {"eeeeeeeee", "EEEEEEE", "4444444444"},
      {"cccc", "uhbnmjh", "48488484"}};
  int amount_of_abonents = 6; //TODO =0
  char title[] = "\n\t\tАбонентский справочник\n\n";
  char menu[] =
      "1) Добавить абонента\n2) Удалить абонента\n3) Поиск абонентов по "
      "имени\n4) Вывод всех записей\n5) Выход\n\n";
  char data[DATA_LENGTH];
  int repeat = 1;
  int menu_choice;

  printf("\n\n\n\n\n\n\n\n");
  while (repeat) {
    printf("\n%s%s", title, menu);
   // printf("amount == %d\n", amount_of_abonents); // TODO DELETE
    printf("Введите пункт меню: ");

    InputData(data);
          if (sscanf(data, "%d", &menu_choice) == 1 && menu_choice > 0 &&
        menu_choice < 6) {
   //   printf("MENU choise == %d\n", menu_choice); // TODO DELETE

      switch (menu_choice) {
      case 1:
        if (amount_of_abonents == MAX_ABONENTS) {
          printf("Справочник переполнен, невозможно добавить абонента\n");
        } else {
          printf("Введите имя абонента:\n");
          InputData(phone_book[amount_of_abonents].name);
          printf("Введите фамилию абонента:\n");
          InputData(phone_book[amount_of_abonents].second_name);
          printf("Введите телефон абонента:\n");
          InputData(phone_book[amount_of_abonents].tel);
          printf("Добавлен абонент:\n");
          OutputAbonentData(phone_book[amount_of_abonents]);
          amount_of_abonents++;
        }
        sleep(SLEEP_SECONDS);
        break;
      case 2: // удаление: на место удаляемой записи копируется последняя по
              // номеру  запись, место в памяти, занимавшая последняя запись
              // обнуляется, количество записей уменьшается на 1
        printf("Введите порядковый номер записи в справочнике, который хотите "
               "удалить:\n");
               InputData(data);
        int index;
        int scanned = sscanf(data, "%d", &index);
        --index;
        if (scanned != 1) {
          printf("Это не номер\n");
        } else if (index < 0 || index >= amount_of_abonents) {
          {
            printf("Нет записи с таким номером\n");
          }
        } else {
        //   phone_book[index] = phone_book[amount_of_abonents - 1]; // since c99
         strncpy(phone_book[index].name , phone_book[amount_of_abonents - 1].name, DATA_LENGTH);
          strncpy(phone_book[index].second_name ,phone_book[amount_of_abonents - 1].second_name, DATA_LENGTH);
          strncpy(phone_book[index].tel , phone_book[amount_of_abonents - 1].tel, DATA_LENGTH);
          for (int i = 0; i < DATA_LENGTH; ++i) {
            phone_book[amount_of_abonents].name[i] = 0;
            phone_book[amount_of_abonents].second_name[i] = 0;
            phone_book[amount_of_abonents].tel[i] = 0;
          }
          --amount_of_abonents;
          printf("Запись удалена\n");
        }
        sleep(SLEEP_SECONDS);
        break;
      case 3:
        printf("Введите имя для поиска:\n");
        InputData(data);
        int found = 0;
        for (int i = 0; i < amount_of_abonents; ++i) {
          if (strncmp(data, phone_book[i].name, DATA_LENGTH) == 0) {
            printf("%3d. ", i);
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
      case 4:
      printf("\nСписок всех абонентов: \n");
        for (int i = 0; i < amount_of_abonents; ++i) {
          printf("%3d. ", i + 1);
          OutputAbonentData(phone_book[i]);
        }
        printf("\n");
        sleep(2*SLEEP_SECONDS);
        break;
      case 5:
        repeat = 0;
        break;
      }
    } else {
      printf("Неправильно введен пункт меню\n");
    }
  } // while (repeat)
  return 0;
} 


void InputData(char *data) {
  char str[MAX_LENGTH];
  fgets(str, MAX_LENGTH, stdin);
  char format[8];
  sprintf(format, "%%%ds", DATA_LENGTH - 1);
  sscanf(str, format, data);
}
