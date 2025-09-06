// #include "server.h"

#include "functions/common_data.h"

int make_mutex_semaphore(int *mutex_sem, const char *file, int proj_id,
                         int num_sems, int init_value);

int main() {
  int shmid_for_client_ids, shmid_names, shmid_chat;
  int mutex_sem_names, mutex_sem_chat;
  int *shmaddress_client_ids;
  char *shmaddr_names;
  ChatData *shmaddr_chat;
  int res =
      make_shared_memory(&shmid_for_client_ids, (void *)&shmaddress_client_ids,
                         (1 + MAX_CLIENTS) * sizeof(int),
                         FILE_TO_KEY_CLIENT_IDS, NUMBER_TO_KEY_CLIENT_IDS) ||
      make_shared_memory(&shmid_names, (void *)&shmaddr_names,
                         (1 + MAX_CLIENTS) * NAME_LEN, FILE_TO_KEY_NAMES,
                         NUMBER_TO_KEY_NAMES) ||
      make_shared_memory(&shmid_chat, (void *)&shmaddr_chat, sizeof(ChatData),
                         FILE_TO_KEY_CHAT, NUMBER_TO_KEY_CHAT);
  error_handle(res, "make shared memory");

  res = make_mutex_semaphore(&mutex_sem_names, FILE_TO_KEY_NAMES,
                             NUMBER_TO_KEY_SEM_NAMES, 1, 0) ||
        make_mutex_semaphore(&mutex_sem_chat, FILE_TO_KEY_CHAT,
                             NUMBER_TO_KEY_SEM_CHAT, 1, 0);
  error_handle(res, "make semaphore");

  shmaddress_client_ids[0] = 1;
  strncpy(shmaddr_names, "ONLINE:", MSG_LEN);
  pid_t process_read_input_pid = fork();
  if (0 == process_read_input_pid) {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    printf(
        "[SERVER WORKS]   Run './client' in another terminal.   Press <ESCAPE> "
        "to exit  ");
    char chr = ' ';
    while (ESCAPE_KEY != chr) {
      chr = getchar();
    }
    nocbreak();
    endwin();
  } else {
    wait(NULL);
    printf("\nAt the end. IN SHARED MEMORY: \n");
    for (int i = 0; i < MAX_NUM_CHAT; i++) {
      printf("-%u-%u---%d--%s--  \n", shmaddr_chat->last, shmaddr_chat->last, i,
             shmaddr_chat->chat[i]);
    }

    for (int i = 0; i < (1 + MAX_CLIENTS); i++) {
      if (shmaddress_client_ids[i])
        printf("%d: %d   ", i, shmaddress_client_ids[i]);
    }
    printf("\nNAMES:\n");
    for (int i = 0; i <= MAX_CLIENTS; i++) {
      if (shmaddress_client_ids[i])
        printf("id=%d;   pid=%d;   name:%s\n", i, shmaddress_client_ids[i],
               shmaddr_names + i * NAME_LEN);
    }
    //===================================================
    printf("shmids: %d %d %d\n", shmid_for_client_ids, shmid_names, shmid_chat);
    shmctl(shmid_for_client_ids, IPC_RMID, NULL);
    shmctl(shmid_names, IPC_RMID, NULL);
    shmctl(shmid_chat, IPC_RMID, NULL);
    semctl(mutex_sem_names, 0, IPC_RMID);
    semctl(mutex_sem_chat, 0, IPC_RMID);
    //===================================================
  }
  printf("SERVER PROCESS #%d ENDS\n", getpid());
  exit(EXIT_SUCCESS);
}

int make_mutex_semaphore(int *mutex_sem, const char *file, int proj_id,
                         int num_sems, int init_value) {
  key_t key = ftok(file, proj_id);
  if (-1 == key) {
    perror("ftok in make semaphore");
    return -1;
  }
  *mutex_sem = semget(key, num_sems, IPC_CREAT | 0660);
  if (-1 == *mutex_sem) {
    perror("semget in make semaphore");
    return -1;
  }
  union semun sem_attributes;
  sem_attributes.val = init_value;
  int res = semctl(*mutex_sem, 0, SETVAL, sem_attributes);
  if (-1 == res) {
    perror("semctl in make semaphore");
    return -1;
  }
  return 0;
}