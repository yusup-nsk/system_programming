#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
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

struct node {
  struct abonent abonent;
  struct node *prev;
  struct node *next;
};

int InputData(char *); /* отсеивает пробельные символы и
считывает первые ( DATA_LENGTH-1 ) символы   в введенном слове */
void OutputAbonentData(struct abonent abonent);
int CountAbonents(struct node *head);
void InputAbonentList(struct node **head, struct node **tail);
void RemoveAbonentFromList(struct node **head, struct node **tail);
int FindAbonentList(const struct node *head);
void OutputAllAbonentsList(const struct node *head);
void FreeAbonentList(struct node **ptr_head, struct node **ptr_tail);
// for testing:
void ReverseOutputAllAbonentsList(const struct node *tail);
// TODO DELETE THIS
int main() {
  struct node *head = NULL;
  struct node *tail = NULL;
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
    if (0 == InputData(data))
      continue;
    if (sscanf(data, "%d", &menu_choice) == 1 && menu_choice > 0 &&
        menu_choice < 6) {
      switch (menu_choice) {
      case 1: // добавление абонента
        InputAbonentList(&head, &tail);
        sleep(SLEEP_SECONDS);
        break;
      case 2: // удаление первой встреченной записи абонента с данными именем и
              // фамилией
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
        ReverseOutputAllAbonentsList(tail);
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

int CountAbonents(struct node *head) {
  if (NULL == head) {
    return 0;
  }
  int amount = 0;
  for (; head; amount++, head = head->next)
    ;
  return amount;
}

void InputAbonentList(struct node **ptr_head, struct node **ptr_tail) {
  if (NULL == ptr_head || NULL == ptr_tail) {
    printf("ERROR: wrong arguments in InputAbonentList\n");
    return;
  }
  if (CountAbonents(*ptr_head) == MAX_ABONENTS) {
    printf("Справочник переполнен, невозможно добавить абонента\n");
  } else {
    if (NULL == *ptr_tail) {
      *ptr_tail = malloc(sizeof(struct node));
      assert(NULL == *ptr_head);
      *ptr_head = *ptr_tail;
      (*ptr_tail)->prev = NULL;
      (*ptr_tail)->next = NULL;
    } else {
      assert(NULL != *ptr_head);
      (*ptr_tail)->next = malloc(sizeof(struct node));
      (*ptr_tail)->next->prev = *ptr_tail;
      (*ptr_tail)->next->next = NULL;
      *ptr_tail = (*ptr_tail)->next;
    }
    if (*ptr_tail) {
      do {
        printf("Введите имя абонента:\n");
      } while (0 == InputData((*ptr_tail)->abonent.name));
      do {
        printf("Введите фамилию абонента:\n");
      } while (0 == InputData((*ptr_tail)->abonent.second_name));
      do {
        printf("Введите телефон абонента:\n");
      } while (0 == InputData((*ptr_tail)->abonent.tel));
      printf("Добавлен абонент:\n");
      OutputAbonentData((*ptr_tail)->abonent);
    } else {
      printf("Can`t allocate memory\n");
    }
  }
}

void RemoveAbonentFromList(struct node **ptr_head, struct node **ptr_tail) {
  if (NULL == ptr_head || NULL == *ptr_head || NULL == ptr_tail ||
      NULL == *ptr_tail) {
    printf("Справочник пуст, нечего удалять\n");
    return;
  }
  char name_for_delete[DATA_LENGTH];
  char second_name_for_delete[DATA_LENGTH];
  int found = 0;
  do {
    printf("Введите имя  абонента, которого хотите "
           "удалить из справочника:\n");
  } while (0 == InputData(name_for_delete));
  do {
    printf("Введите фамилию  абонента, которого хотите "
           "удалить из справочника:\n");
  } while (0 == InputData(second_name_for_delete));

  for (struct node *p = *ptr_head; p;) {
    if (strncmp(p->abonent.name, name_for_delete, DATA_LENGTH) == 0 &&
        strncmp(p->abonent.second_name, second_name_for_delete, DATA_LENGTH) ==
            0) {
      found++;
      if (p == *ptr_tail) {
        if (p == *ptr_head) {
          *ptr_tail = NULL;
          *ptr_head = NULL;
        } else {
          *ptr_tail = (*ptr_tail)->prev;
          (*ptr_tail)->next = NULL;
        }
      } else {
        if (p == *ptr_head) {
          *ptr_head = (*ptr_head)->next;
          (*ptr_head)->prev = NULL;
        } else {
          (p->prev)->next = p->next;
          (p->next)->prev = p->prev;
        }
      }
      free(p);
      p = NULL;
    }
    if (p)
      p = p->next;
  }
  if (found)
    printf("%d запись удалена\n", found);
  else {
    printf("Не найден абонент с именем \"%s\" и фамилией \"%s\"\n",
           name_for_delete, second_name_for_delete);
  }
}

int FindAbonentList(const struct node *head) {
  if (NULL == head) {
    printf("Справочник пуст, нечего искать\n");
    return 0;
  }
  char name_for_seek[DATA_LENGTH];
  do {
    printf("Введите имя для поиска:\n");
    InputData(name_for_seek);
  } while ('\0' == name_for_seek[0]);
  int found = 0;
  for (const struct node *p = head; p; p = p->next) {
    if (strncmp(p->abonent.name, name_for_seek, DATA_LENGTH) == 0) {
      OutputAbonentData(p->abonent);
      found++;
    }
  }
  if (found) {
    printf("Найдено %d записей абонентов с именем \"%s\"\n", found,
           name_for_seek);
  } else {
    printf("Не найдено ни одной записи с именем \"%s\"\n", name_for_seek);
  }
  return found;
}

void OutputAllAbonentsList(const struct node *head) {
  if (NULL == head) {
    printf("Справочник пуст, нечего выводить\n");
    return;
  }
  printf("\nСписок всех абонентов: \n");
  int i = 0;
  while (head) {
    printf("%3d. ", ++i);
    OutputAbonentData(head->abonent);
    head = head->next;
  }
  printf("\n");
}

void ReverseOutputAllAbonentsList(const struct node *tail) {
  if (NULL == tail) {
    printf("Справочник пуст, нечего выводить\n");
    return;
  }
  printf("\nСписок всех абонентов: \n");
  int i = 0;
  while (tail) {
    printf("%3d. ", ++i);
    OutputAbonentData(tail->abonent);
    tail = tail->prev;
  }
  printf("\n");
}

void FreeAbonentList(struct node **ptr_head, struct node **ptr_tail) {
  if (NULL == ptr_head || NULL == ptr_tail || NULL == *ptr_head ||
      NULL == *ptr_tail)
    return;
  for (struct node *next_ptr = *ptr_head;
       next_ptr != *ptr_tail; 
  ) {
    struct node *current = next_ptr;
    next_ptr = next_ptr->next;
    current->prev = NULL;
    current->next = NULL;
    free(current);
  }
  free(*ptr_tail);
  *ptr_head = NULL;
  *ptr_tail = NULL;
}