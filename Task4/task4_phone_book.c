#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_ABONENTS 10
#define DATA_LENGTH 12
#define MAX_LENGTH 256

struct Abonent {
  char name[DATA_LENGTH];
  char second_name[DATA_LENGTH];
  char tel[DATA_LENGTH];
};

void InputStr(char *str);
void InputStr2(char *str);

void OutputAbonentData(struct Abonent abonent) {
  printf("Имя: %-*s   Фамилия: %-*s   Телефон: %-*s\n", DATA_LENGTH,
         abonent.name, DATA_LENGTH, abonent.second_name, DATA_LENGTH,
         abonent.tel);
};


int main() {
  struct Abonent phone_book[MAX_ABONENTS] = {
      {"aaaaa", "AAAAAAAA", "0000000"},
      {"bbbbbbbbb", "BBBBBBBB", "1111111111"},
      {"cccc", "CCCCCCC", "2222222222"},
      {"ddddddddddd", "DDDDDDDD", "3333333"},
      {"eeeeeeeee", "EEEEEEE", "4444444444"},
      {"cccc", "uhbnmjh", "48488484"}};
  int amount_of_abonents = 6;
  char title[] = "\t\tАбонентский справочник\n";
  char menu[] =
      "1) Добавить абонента\n2) Удалить абонента\n3) Поиск абонентов по "
      "имени\n4) Вывод всех записей\n5) Выход\n";
  char string[MAX_LENGTH];
  int num;
  int repeat = 1;
  int menu_choice;
  char ch = ' ';

  printf("\n\n\n\n\n\n\n\n");
 // printf("%s%s", title, menu);
  while (repeat) {
     printf("%s%s", title, menu);
    printf("amount == %d\n", amount_of_abonents);
    printf("Введите пункт меню: ");
    // if (scanf("%d%c", &menu_choice,&ch) == 2 && ch!= '\n' && menu_choice > 0
    // && menu_choice < 6) {
    fgets(string, MAX_LENGTH, stdin);
    printf("string == ^%s$\n", string);
    if (sscanf(string, "%d", &menu_choice) == 1 && menu_choice > 0 &&
        menu_choice < 6) {
      // if (scanf("%d", &menu_choice) == 1 && menu_choice > 0 && menu_choice <
      // 6) {
      printf("MENU choise == %d\n", menu_choice);
      switch (menu_choice) {
      case 1:
        if (amount_of_abonents == MAX_ABONENTS) {
          printf("Справочник переполнен, невозможно добавить абонента\n");
        } else {
          printf("Введите имя абонента:\n");
          // fgets(phone_book[amount_of_abonents].name, DATA_LENGTH +1,
          // stdin);
          //  InputData(phone_book[amount_of_abonents].name);
          InputStr(phone_book[amount_of_abonents].name);
          // scanf("%s", phone_book[amount_of_abonents].name);

          printf("^%s$", phone_book[amount_of_abonents].name);
          //  OutputData(phone_book[amount_of_abonents].name);
          amount_of_abonents++;
        }

        break;
      case 2:
        printf("Введите порядковый номер записи в справочнике, который хотите "
               "удалить\n");
        int index;
        scanf("%d", &index);
        --index;
        if (index < 0 || index >= amount_of_abonents) {
          printf("Нет записи с таким номером\n");
        } else {
          phone_book[index] = phone_book[amount_of_abonents - 1];
          for (int i = 0; i < DATA_LENGTH; ++i) {
            phone_book[amount_of_abonents].name[i] = 0;
            phone_book[amount_of_abonents].second_name[i] = 0;
            phone_book[amount_of_abonents].tel[i] = 0;
          }
          --amount_of_abonents;
        }

        break;
      case 3:
        printf("Введите имя для поиска:\n");
        char tmp[MAX_LENGTH];
        char s[DATA_LENGTH];

        // InputStr2(s);
       // scanf("%s", s);
          fgets(tmp, MAX_LENGTH, stdin);
          strncpy(s,tmp, DATA_LENGTH);
          char *pointer = strchr(s,'\n');
          *pointer = '\0';
        printf("^%s$\n", s);
        int found = 0;
        for (int i = 0; i < amount_of_abonents; ++i) {
          if (strncmp(s, phone_book[i].name, DATA_LENGTH) == 0) {
            printf("%3d. ", i);
            OutputAbonentData(phone_book[i]);
            found = 1;
          }
        }
        if (!found) {
          printf("Не найдено ни одной записи с именем %s\n", s);
        }
        break;
      case 4:
        for (int i = 0; i < amount_of_abonents; ++i) {
          printf("%3d. ", i + 1);
          OutputAbonentData(phone_book[i]);
          // printf("Имя:%s   Фамилия: %s   Телефон:  %s", phone_book[i].name,
          //        phone_book[i].second_name, phone_book[i].tel);
        }
        sleep(0);
       // printf("%s%s", title, menu);
        break;
      case 5:
        repeat = 0;
        break;
      }
    } else {
      printf("Неправильный номер\n");
      sleep(0);
    }
    sleep(1);
  }
  return 0;
}

void InputStr(char *str) {
  // scanf("%*c");
  fgets(str, MAX_LENGTH, stdin);
  char *p = str;
  for (; *p; p++)
    ;
  p--;
  if ('\n' == *p)
    *p = '\0';
}

void InputStr2(char *s) {
  char str[MAX_LENGTH];
  fgets(str, MAX_LENGTH, stdin);
  int i = 0;
  for (; i < DATA_LENGTH - 1; ++i) {
    if ('\n' == str[i]) {
      s[i] = '\0';
      break;
    } else
      s[i] = str[i];
  }
  s[i] = '\0';
}
