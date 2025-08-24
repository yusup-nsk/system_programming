
#include "functions/windows_procedures.h"

int g_chahged_screen_size = 0;

int main() {
  int msqid = get_msqid(NUMBER_TO_KEY);
  if (msqid < 0) {
    perror("get_msqid");
    exit(EXIT_FAILURE);
  }
  char name[NAME_LEN];
  printf("Welcome to the CHAT ROOM.\n Enter your name: ");
  fgets(name, NAME_LEN, stdin);
  if (strchr(name, '\n'))
    *(strchr(name, '\n')) = '\0';
  else {
    name[NAME_LEN - 1] = '\0';
  }
  Data_of_client clientdata = {0};
  strncpy(clientdata.name, name, NAME_LEN);
  initscr();
  struct msgbuff message = {0};
  msgrcv(msqid, &message, sizeof(struct msgbuff), PRIO_SERVER_TO_CLIENT, 0);
  sscanf(message.text_msg, "%u", &(clientdata.id));
  sprintf(message.text_msg, "%s %u", clientdata.name, clientdata.id);
  message.prioritet = PRIO_CLIENT_TO_SERVER;
  message.text_msg[MSG_LEN - 1] = '\0';
  int res_msgsnd = msgsnd(msqid, &message, sizeof(message), IPC_NOWAIT);
  if (res_msgsnd == -1) {
    perror("sending message");
    exit(EXIT_FAILURE);
  }
  int individual_queue = -1;
  for (int tryings = 0; individual_queue < 0 && tryings < MAX_TRY_CONNECT;
       tryings++, usleep(MSEC_BETWEEN_TRY_CONNECT)) {
    individual_queue = get_msqid(NUMBER_TO_KEY + clientdata.id);
  }
  if (individual_queue < 0) {
    perror("getting individual queue");
    exit(EXIT_FAILURE);
  }
  int mqueue_inner = make_msqid(2 * (NUMBER_TO_KEY + clientdata.id));
  pid_t main_pid = getpid();
  pid_t pid = fork();

  if (0 == pid) {  // процесс для считывания с клавиатуры и
    // отправки сообщения серверу
    char str[MSG_LEN] = {0};
    unsigned indx = 0;
    cbreak();
    noecho();
    initscr();
    keypad(stdscr, TRUE);
    while (1) {  // while not inputted ESCAPE_KEY
      int ch;
      if ((ch = getch()) == ESCAPE_KEY) {
        break;
      }
      if (KEY_BACKSPACE == ch) {
        if (indx) {
          str[--indx] = 0;
          message.prioritet = 1;
          sprintf(message.text_msg, "%s", str);
          msgsnd(mqueue_inner, &message, sizeof(message), IPC_NOWAIT);
        }
      } else {
        str[indx] = ch;
        if ('\n' == ch) {
          message.prioritet = 1;
          message.text_msg[0] = '\n';
          message.text_msg[1] = 0;
          msgsnd(mqueue_inner, &message, sizeof(message), IPC_NOWAIT);

          str[indx] = 0;
          indx = 0;
          if (strlen(str)) {
            message.prioritet = PRIO_CLIENT_TO_SERVER;
            strncpy(message.text_msg, str, MSG_LEN);
            memset(str, 0, MSG_LEN);
            res_msgsnd =
                msgsnd(individual_queue, &message, sizeof(message), IPC_NOWAIT);
            if (res_msgsnd == -1) {
              perror("sending message 2");
              exit(EXIT_FAILURE);
            }
          }
        } else {
          message.prioritet = 1;
          sprintf(message.text_msg, "%s", str);
          msgsnd(mqueue_inner, &message, sizeof(message), IPC_NOWAIT);
          indx++;
        }
      }
    }
    kill(main_pid, SIGTERM);
  } else {  // главный процесс получает от сервера данные о чате и именах
    int inputted = 0;
    WINDOW *the_window[3];
    Frame the_frame[3];
    FILE *errors_log = freopen("errors.log", "w", stderr);
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    refresh();
    init_pair(1, COLOR_YELLOW, COLOR_BLUE);
    init_pair(2, COLOR_RED, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    windows_initiation(the_window, the_frame);
    signal(SIGWINCH, sig_winch);
    struct chat_msg chatmsg;
    unsigned start_chat = 0;
    wprintw(the_window[INPUT_WINDOW],
            "[%s]: Press <ESCAPE> to exit the CHAT ROOM \n "
            "or type your MESSAGE to start talking"
            "\n",
            clientdata.name);
    wrefresh(the_window[INPUT_WINDOW]);
    wmove(the_window[INPUT_WINDOW], 0, 0);
    wprintw(the_window[INPUT_WINDOW], "[%s]:\n", clientdata.name);
    // int repeat = 1;
    while (1) {
      if (g_chahged_screen_size) {
        g_chahged_screen_size = 0;
        process_change_screen_size(the_window, the_frame, clientdata,
                                   &start_chat);
        sleep(1);
      }
      if (msgrcv(mqueue_inner, &message, sizeof(message), 1, IPC_NOWAIT) !=
          -1) {
        inputted = 1;
        wmove(the_window[INPUT_WINDOW], 0, 0);
        wprintw(the_window[INPUT_WINDOW], "\n\n");
        wmove(the_window[INPUT_WINDOW], 0, 0);
        wprintw(the_window[INPUT_WINDOW], "[%s]: %s", name, message.text_msg);
      }
      if (inputted) wrefresh(the_window[INPUT_WINDOW]);

      if (msgrcv(individual_queue, &chatmsg, sizeof(chatmsg),
                 PRIO_SERVER_TO_CLIENT, IPC_NOWAIT) != -1) {
        switch (chatmsg.text_msg[0]) {
          case SYMBOL_CHAT:
            strncpy(clientdata.chat[clientdata.size_chat++],
                    chatmsg.text_msg + 1, CHAT_MSG_LEN);
            break;
          case SYMBOL_NEW_NAME:
            strncpy(clientdata.other_names[clientdata.size_names++],
                    chatmsg.text_msg + 1, NAME_LEN);
            break;
        }
      }
      output_chat_and_names_windows(the_window, clientdata, the_frame,
                                    &start_chat);
      refresh();
    }
    delwin(the_window[0]);
    delwin(the_window[1]);
    delwin(the_window[2]);
    endwin();
    if (errors_log) fclose(errors_log);
  }
  exit(EXIT_SUCCESS);
}
