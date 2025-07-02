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
//  int amount;
};

int InputData(char *); /* отсеивает пробельные символы и
считывает первые ( DATA_LENGTH-1 ) символы   в введенном слове */
void OutputAbonentData(struct abonent abonent);
int CountAbonents(struct  node *head);
void InputAbonentList(struct  node **head, struct  node ** tail);
void RemoveAbonentFromList(struct  node ** head, struct node ** tail);
int FindAbonentList(const struct  node * head);
void OutputAllAbonentsList(const struct  node * head);
void FreeAbonentList(struct  node **ptr_head, struct  node ** ptr_tail);



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
        RemoveAbonentFromList(&head, &tail);
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
  FreeAbonentList(&head, &tail);
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

void RemoveAbonentFromList(struct  node ** ptr_head, struct node ** ptr_tail){
  if (NULL == ptr_head || NULL == *ptr_head) {
    printf("Справочник пуст, нечего удалять\n");
    return;
  }
   printf("Введите имя  абонента, которого хотите "
         "удалить из справочника:\n");
  char data[DATA_LENGTH];
  char name_for_delete[DATA_LENGTH];
  char second_name_for_delete[DATA_LENGTH];
  InputData(data);
  int scanned = sscanf(data, "%s", name_for_delete);
  //  if (scanned != 1){

  //  }
   printf("Введите фамилию  абонента, которого хотите "
         "удалить из справочника:\n");
  InputData(data);
   scanned = sscanf(data, "%s", second_name_for_delete);         
  // if (scanned != 1) {
  //   printf("Это не номер\n");
  // } else 
  for(struct node * p= *ptr_head; p; p=p->next){
    if (strncmp(p->abonent.name, name_for_delete, DATA_LENGTH)==0 && strncmp(p->abonent.second_name, second_name_for_delete, DATA_LENGTH)==0){
      if (p==*ptr_tail){
        *ptr_tail=(*ptr_tail)->prev;
        (*ptr_tail)->next=NULL;
                free(p);
                p=NULL;
      }else if (p==*ptr_head) {
        *ptr_head=(*ptr_head)->next;

      }
    }
  }

    printf("Запись удалена\n"); 
}

int FindAbonentList(const struct  node * head){
  if ( NULL == head) {
    printf("Справочник пуст, нечего искать\n");
    return 0;
  }
  char name_for_seek[DATA_LENGTH]; 
  do {
    printf("Введите имя для поиска:\n");
    InputData(name_for_seek);    
  } while (name_for_seek[0] == '\0');
 int found = 0;
  for(const struct node * p= head; p; p=p->next){
    if (strncmp(p->abonent.name, name_for_seek, DATA_LENGTH)==0){
      OutputAbonentData(p->abonent);
      found++;
    }
  }
  if (found) {
    printf("Найдено %d записей абонентов с именем \"%s\"\n", found, name_for_seek);
  }
  else {
    printf("Не найдено ни одной записи с именем \"%s\"\n", name_for_seek);
  }
  return found;
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

void FreeAbonentList(struct  node **ptr_head, struct  node ** ptr_tail)
{
  *ptr_tail = NULL;
  //struct node * free_this_ptr=NULL;
   struct node * next_ptr=*ptr_head;
    struct node * current = NULL;
   for(; next_ptr; next_ptr = next_ptr->next){
    if (current) {free(current);
    }
    current = next_ptr;
   }
  *ptr_head = NULL;
}