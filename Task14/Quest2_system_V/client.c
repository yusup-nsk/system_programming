
#include "functions/client_functions.h"

int g_chahged_screen_size = 0;

int broadcast_signal_to_all_clients(int *shmaddr_array_id_and_pid) {
  int res = 0;
  for (unsigned p = 1; !res && p <= MAX_CLIENTS; p++) {
    if (shmaddr_array_id_and_pid[p]) {
      res = kill(shmaddr_array_id_and_pid[p], SIGUSR1);
    }
  }
  return res;
}

int main() {
  FILE *errors_log = freopen("errors.log", "a+", stderr);
  int shmid_client_ids, shmid_names, shmid_chat;
  int mutex_sem_names, mutex_sem_chat;
  int *shmaddr_array_id_and_pid;  //данная разделяемая память используется как
                                  //массив, где индекс - это ID клиента, а
                                  //значение по этому индексу - PID процесса
                                  //клиента.
  char *shmaddr_names;
  ChatData *shmaddr_chat;
  int res =
      get_shared_memory(&shmid_client_ids, (void *)&shmaddr_array_id_and_pid,
                        (1 + MAX_CLIENTS) * sizeof(int), FILE_TO_KEY_CLIENT_IDS,
                        NUMBER_TO_KEY_CLIENT_IDS) ||
      get_shared_memory(&shmid_names, (void *)&shmaddr_names,
                        (1 + MAX_CLIENTS) * NAME_LEN, FILE_TO_KEY_NAMES,
                        NUMBER_TO_KEY_NAMES) ||
      get_shared_memory(&shmid_chat, (void *)&shmaddr_chat, sizeof(ChatData),
                        FILE_TO_KEY_CHAT, NUMBER_TO_KEY_CHAT);
  error_handle(res, "get shared memory");
  res = get_mutex_semaphore(&mutex_sem_names, FILE_TO_KEY_NAMES,
                            NUMBER_TO_KEY_SEM_NAMES) ||
        get_mutex_semaphore(&mutex_sem_chat, FILE_TO_KEY_CHAT,
                            NUMBER_TO_KEY_SEM_CHAT);
  error_handle(res, "get semaphore");
  struct sembuf lock[2] = {{0, 0, 0}, {0, 1, 0}};
  struct sembuf unlock[1] = {{0, -1, 0}};
  char name[NAME_LEN];
  int mypid = getpid();
  int my_client_id = -1;

  int have_not_id = 1;
  while (have_not_id) {
    int id = 1;
    for (; id <= MAX_CLIENTS; id++) {
      if (mypid == shmaddr_array_id_and_pid[id]) {
        my_client_id = id;
        have_not_id = 0;
        break;
      }
      if (0 == shmaddr_array_id_and_pid[id]) {
        shmaddr_array_id_and_pid[id] = mypid;
        // СПОСОБ РАЗРЕШЕНИЯ ГОНКИ без примитивов синхронизации:
        // возможен случай, что здесь другой клиент перезапишет эту ячейку
        // памяти, поэтому цикл повторяется, пока не удастся записать свой PID в
        // следующую пустующую ячейку
        break;
      }
    }
    if (MAX_CLIENTS + 1 ==
        id) {  // все ячейки массива заняты, когда какой-нибудь клиент выйдет из
               // чата, он обнулит значение в массиве  и новый клиент сможет
               // занять это значение ID.
      printf("Sorry, no place for the client, try again later\n");
      exit(EXIT_SUCCESS);
    }
  }
  input_name(name);
  //"""""""""""""""""""" lock """"""""""""""""""""""""""""""""""""
  semop(mutex_sem_names, lock, 2);
  strncpy(shmaddr_names + NAME_LEN * my_client_id, name, NAME_LEN);
  (shmaddr_names + NAME_LEN * my_client_id)[NAME_LEN - 1] = '\0';
  semop(mutex_sem_names, unlock, 1);
  //""""""""""""""""""""""" unlock """""""""""""""""""""""""""""""""
  int shmid_inner;
  char *shmaddr_inner;  // общая память внутренних процессов одного клиента
  res =
      make_shared_memory(&shmid_inner, (void *)&shmaddr_inner, CHAT_MSG_LEN + 2,
                         FILE_TO_KEY, NUMBER_TO_KEY + mypid);
  error_handle(res, "make shared memory");

  cbreak();
  noecho();
  initscr();
  Data_of_client clientdata = {0};
  clientdata.id = my_client_id;
  strncpy(clientdata.name, name, NAME_LEN);

  pid_t pid = fork();
  if (0 == pid) {  // child process for input messages
    res = broadcast_signal_to_all_clients(
        shmaddr_array_id_and_pid);  // здесь не использую примитивы
                                    // синхронизации, потому что не важно какой
                                    // клиент пошлет сигналы всем главным
                                    // процессам клиентов
    for (unsigned p = 1; !res && p <= MAX_CLIENTS; p++) {
      if (shmaddr_array_id_and_pid[p]) {
        res = kill(shmaddr_array_id_and_pid[p], SIGUSR1);
      }
    }
    int repeat = 1;
    while (!res && repeat) {  // процесс считывания  с клавиатуры и
      // отправки сообщения в общую память
      char str[MSG_LEN] = {0};
      unsigned indx = 0;
      noecho();
      keypad(stdscr, TRUE);
      while (1) {  // while not inputted ESCAPE_KEY
        int ch;
        if ((ch = getch()) == ESCAPE_KEY) {
          repeat = 0;
          break;
        }
        if (KEY_BACKSPACE == ch) {
          if (indx) {
            str[--indx] = 0;
            sprintf(shmaddr_inner, "%c[%s]: %s", SYMBOL_WRITING_MESSAGE, name,
                    str);
          }
        } else {
          if ('\n' == ch) {
            str[indx] = 0;
            indx = 0;
            if (strlen(str)) {
              //"""""""""""""""""""" lock """"""""""""""""""""""""""""""""""""
              semop(mutex_sem_chat, lock, 2);
              send_message_to_chat(shmaddr_chat, name, str);
              broadcast_signal_to_all_clients(shmaddr_array_id_and_pid);
              semop(mutex_sem_chat, unlock, 1);
              //""""""""""""""""""""""" unlock """""""""""""""""""""""""""""""""
            }
            memset(shmaddr_inner, 0, CHAT_MSG_LEN);
            shmaddr_inner[0] = SYMBOL_SEND_MESSAGE;
          } else {
            str[indx] = ch;
            str[indx + 1] = 0;
            sprintf(shmaddr_inner, "%c[%s]: %s", SYMBOL_WRITING_MESSAGE, name,
                    str);
            indx++;
          }
        }
        kill(mypid, SIGUSR1);
      }
      shmaddr_inner[0] = SYMBOL_EXIT;
      // res = broadcast_signal_to_all_clients(shmaddr_array_id_and_pid);
    }
  } else {  // pid!=0, main process
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    int ret = sigprocmask(SIG_BLOCK, &set, NULL);
    error_handle(ret < 0, "set blocking signal SIGUSR1\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    int inputted = 0;
    WINDOW *the_window[3];
    Frame the_frame[3];
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    refresh();
    init_pair(1, COLOR_YELLOW, COLOR_BLUE);
    init_pair(2, COLOR_RED, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    windows_initiation(the_window, the_frame);
    signal(SIGWINCH, sig_winch);

    unsigned start_chat = 0;
    //   shmaddr_chat->overflowed ? (shmaddr_chat->last + 1) % MAX_NUM_CHAT : 0;

    wprintw(the_window[INPUT_WINDOW],
            "[%s]: Press <ESCAPE> to exit the CHAT ROOM "
            "or type your MESSAGE to start talking"
            "\n",
            name);
    wrefresh(the_window[INPUT_WINDOW]);
    wmove(the_window[INPUT_WINDOW], 0, 0);
    wprintw(the_window[INPUT_WINDOW], "[%s]:\n", name);
    while (1) {
      if (g_chahged_screen_size) {
        g_chahged_screen_size = 0;
        process_change_screen_size3(the_window, the_frame);
      }

      if (SYMBOL_EXIT == shmaddr_inner[0]) {
        break;
      }
      if (SYMBOL_WRITING_MESSAGE == shmaddr_inner[0]) {
        inputted = 1;
        wmove(the_window[INPUT_WINDOW], 0, 0);
        wprintw(the_window[INPUT_WINDOW], "\n\n");
        wmove(the_window[INPUT_WINDOW], 0, 0);
        wprintw(the_window[INPUT_WINDOW], "%s", shmaddr_inner + 1);
      } else if (SYMBOL_SEND_MESSAGE == shmaddr_inner[0]) {
        wmove(the_window[INPUT_WINDOW], 0, 0);
        wprintw(the_window[INPUT_WINDOW], "\n\n");
        wmove(the_window[INPUT_WINDOW], 0, 0);
        wprintw(the_window[INPUT_WINDOW], "[%s]:\n\n", name);
      }
      if (inputted) wrefresh(the_window[INPUT_WINDOW]);

      //"""""""""""""""""""" lock """"""""""""""""""""""""""""""""""""
      semop(mutex_sem_chat, lock, 2);
      clientdata.size_chat =
          shmaddr_chat->overflowed ? MAX_NUM_CHAT : shmaddr_chat->last;
      for (unsigned i = 0; i < clientdata.size_chat; i++) {
        if (shmaddr_chat->overflowed)
          strncpy(clientdata.chat[i],
                  shmaddr_chat->chat[(i + shmaddr_chat->last) % MAX_NUM_CHAT],
                  CHAT_MSG_LEN);
        else
          strncpy(clientdata.chat[i], shmaddr_chat->chat[i], CHAT_MSG_LEN);
      }
      semop(mutex_sem_chat, unlock, 1);
      //""""""""""""""""""""""" unlock """""""""""""""""""""""""""""""""

      clientdata.size_names = MAX_CLIENTS + 1;
      //"""""""""""""""""""" lock """"""""""""""""""""""""""""""""""""
      semop(mutex_sem_names, lock, 2);
      for (unsigned i = 0; i <= MAX_CLIENTS; i++) {
        strncpy(clientdata.other_names[i], shmaddr_names + i * NAME_LEN,
                NAME_LEN);
      }
      semop(mutex_sem_names, unlock, 1);
      //""""""""""""""""""""""" unlock """""""""""""""""""""""""""""""""
      output_chat_and_names_windows2(the_window, clientdata, the_frame,
                                     &start_chat);
      refresh();
      int accessed_signal;
      sigwait(&set, &accessed_signal);
    }
    //"""""""""""""""""""" lock """"""""""""""""""""""""""""""""""""
    semop(mutex_sem_chat, lock, 2);
    send_message_to_chat(shmaddr_chat, name, "<<leaves this chat room>>");
    broadcast_signal_to_all_clients(shmaddr_array_id_and_pid);
    semop(mutex_sem_chat, unlock, 1);
    //""""""""""""""""""""""" unlock """""""""""""""""""""""""""""""""

    delwin(the_window[0]);
    delwin(the_window[1]);
    delwin(the_window[2]);
    endwin();
    shmctl(shmid_inner, IPC_RMID, NULL);
    if (errors_log) fclose(errors_log);
    // при выходе удаляем свои данные из массива для установления ID
    // клиента, чтобы новые клиенты могли его использовать
    shmaddr_array_id_and_pid[my_client_id] = 0;
    shmdt(shmaddr_array_id_and_pid);
    shmdt(shmaddr_names);
    shmdt(shmaddr_chat);
  }
  exit(EXIT_SUCCESS);
}
