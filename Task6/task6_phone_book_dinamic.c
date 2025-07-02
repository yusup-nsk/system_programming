#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_ABONENTS 4 //TODO =100
#define DATA_LENGTH 10 //длина строки с данными вместе с '\0'
#define MAX_LENGTH 256
#define SLEEP_SECONDS 2 // пауза в секундах

struct abonent {
  char name[DATA_LENGTH];
  char second_name[DATA_LENGTH];
  char tel[DATA_LENGTH];
};

struct node {
  struct abonent abonent;
  struct node * prev;
  struct  node * next;
  int amount;
};

int InputData(char *); /* отсеивает пробельные символы и
считывает первые ( DATA_LENGTH-1 ) символы   в введенном слове */
void OutputAbonentData(struct abonent abonent);
int CountAbonents(struct  node *head);
void InputAbonentList(struct  node **head, struct  node ** tail);
void RemoveAbonentList(struct  node * head);
int FindAbonentList(const struct  node * head);
void OutputAllAbonentsList(const struct  node * head);

int main() {
  struct abonent phone_book[MAX_ABONENTS];
  int amount_of_abonents = 0;
  struct  node * head = NULL;
  struct  node * tail = NULL;
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
        InputAbonentList(&head,&tail);
        sleep(SLEEP_SECONDS);
        break;
      case 2: // удаление: на место удаляемой записи копируется последняя по
        // номеру  запись, место в памяти, занимавшая последняя запись
        // обнуляется, количество записей уменьшается на 1
        RemoveAbonentList(head);
        sleep(SLEEP_SECONDS);
        break;
      case 3: // поиск по имени
        FindAbonentList(head);
        sleep(SLEEP_SECONDS);
        break;
      case 4: // вывод списка абонентов
        OutputAllAbonentsList(head);
        sleep(2 * SLEEP_SECONDS);
        break;
      case 5: // выход из программы
        repeat = 0;
        break;
      }
    } else {
      printf("Неправильно введен пункт меню\n");
      sleep(2 * SLEEP_SECONDS);
    }
  } // while


  printf("======%d=====\n", CountAbonents(head));
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

void OutputAbonentData(struct abonent abonent) {
  printf("Имя: %-*s  Фамилия: %-*s  Телефон: %-*s\n", DATA_LENGTH, abonent.name,
         DATA_LENGTH, abonent.second_name, DATA_LENGTH, abonent.tel);
}


int CountAbonents(struct  node *head)
{
  int amount = 0;
  for(;head; amount++, head=head->next);
  return amount;
}




void InputAbonent(struct abonent phone_book[MAX_ABONENTS],
                  int *ptr_amount_of_abonents) {
  if (*ptr_amount_of_abonents == MAX_ABONENTS) {
    printf("Справочник переполнен, невозможно добавить абонента\n");
  } else {
    printf("Введите имя абонента:\n");
    while (InputData(phone_book[*ptr_amount_of_abonents].name) == 0)
      ;
    printf("Введите фамилию абонента:\n");
    while (InputData(phone_book[*ptr_amount_of_abonents].second_name) == 0)
      ;
    printf("Введите телефон абонента:\n");
    while (InputData(phone_book[*ptr_amount_of_abonents].tel) == 0)
      ;
    printf("Добавлен абонент:\n");
    OutputAbonentData(phone_book[*ptr_amount_of_abonents]);
    (*ptr_amount_of_abonents)++;
  }
}




void InputAbonentList(struct  node **ptr_head, struct  node ** ptr_tail){
  if(NULL == ptr_head || NULL == ptr_tail) {printf("ERROR: wrong arguments in InputAbonentList\n");
  return;}
  if (CountAbonents(*ptr_head) == MAX_ABONENTS) {
    printf("Справочник переполнен, невозможно добавить абонента\n");
  } else {
    if (NULL==*ptr_tail){
      *ptr_tail=malloc(sizeof(struct node)); 
      *ptr_head = *ptr_tail;
      (*ptr_tail)->prev=NULL;
      (*ptr_tail)->next=NULL;
    } else {
    (*ptr_tail)->next = malloc(sizeof(struct node)); 
    *ptr_tail = (*ptr_tail)->next;
          (*ptr_tail)->prev=NULL;
      (*ptr_tail)->next=NULL;
    }
    if (*ptr_tail){    
    printf("Введите имя абонента:\n");
    while (InputData((*ptr_tail)->abonent.name) == 0)
      ;
    printf("Введите фамилию абонента:\n");
    while (InputData((*ptr_tail)->abonent.second_name) == 0)
      ;
    printf("Введите телефон абонента:\n");
    while (InputData((*ptr_tail)->abonent.tel) == 0)
      ;
    printf("Добавлен абонент:\n");
    OutputAbonentData((*ptr_tail)->abonent);
    // (*ptr_amount_of_abonents)++;
    }
    else {
      printf("Can`t malloc\n");
    }
  }
}
void RemoveAbonentList(struct  node * head){

}

void RemoveAbonent(struct abonent phone_book[MAX_ABONENTS],
                   int *ptr_amount_of_abonents) {
  if (*ptr_amount_of_abonents == 0) {
    printf("Справочник пуст, нечего удалять\n");
    return;
  }
  printf("Введите порядковый номер записи в справочнике, который хотите "
         "удалить:\n");
  char data[DATA_LENGTH];
  InputData(data);
  int index;
  int scanned = sscanf(data, "%d", &index);
  --index;
  if (scanned != 1) {
    printf("Это не номер\n");
  } else if (index < 0 || index >= *ptr_amount_of_abonents) {
    { printf("Нет записи с таким номером\n"); }
  } else {
    strncpy(phone_book[index].name,
            phone_book[*ptr_amount_of_abonents - 1].name, DATA_LENGTH);
    strncpy(phone_book[index].second_name,
            phone_book[*ptr_amount_of_abonents - 1].second_name, DATA_LENGTH);
    strncpy(phone_book[index].tel, phone_book[*ptr_amount_of_abonents - 1].tel,
            DATA_LENGTH);
    for (int i = 0; i < DATA_LENGTH; ++i) {
      phone_book[*ptr_amount_of_abonents - 1].name[i] = 0;
      phone_book[*ptr_amount_of_abonents - 1].second_name[i] = 0;
      phone_book[*ptr_amount_of_abonents - 1].tel[i] = 0;
    }
    (*ptr_amount_of_abonents)--;
    printf("Запись удалена\n");
  }
}

int FindAbonent(const struct abonent phone_book[MAX_ABONENTS],
                int amount_of_abonents) {
  if (amount_of_abonents == 0) {
    printf("Справочник пуст, нечего искать\n");
    return 0;
  }
  char data[DATA_LENGTH];
  do {
    printf("Введите имя для поиска:\n");
    InputData(data);
  } while (data[0] == '\0');
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
  return found;
}

int FindAbonentList(const struct  node * head){


}



void OutputAllAbonentsList(const struct  node * head){
  if (NULL == head) {
    printf("Справочник пуст, нечего выводить\n");
    return;
  }
  printf("\nСписок всех абонентов: \n");
  int i = 0;
  while(head){
     printf("%3d. ", i + 1);
    OutputAbonentData(head->abonent);
    head = head->next;
    i++;
  }
  printf("\n");
}
