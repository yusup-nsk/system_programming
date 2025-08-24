
#include "server.h"

int main() {
  struct msgbuff message1 = {0}, message2 = {0};
  message1.prioritet = PRIO_SERVER_TO_CLIENT;

  int msqid = make_msqid(NUMBER_TO_KEY);
  int msqid_inner = make_msqid(NUMBER_TO_KEY - 1);
  if (msqid < 0 || msqid_inner < 0) {
    perror("make_msqid");
    exit(EXIT_FAILURE);
  }
  pid_t pid = fork();
  if (0 == pid) { /* в этом процессе своя  очередь для первого контакта.
      сервер ставит в очередь сообщений очередной номер id,
      клиент считывает и возвращает свое имя и этот id.
      этот процесс передает главному процессу через внутреннюю очередь сообщений
      данные нового клиента
      */
    unsigned num_clients = 0;
    unsigned id = 11;
    while (1) {
      sprintf(message1.text_msg, "%u", id++);
      int res_msgsnd = msgsnd(msqid, &message1, sizeof(message1), 0);
      if (res_msgsnd == -1) {
        perror("sending message 1");
        exit(EXIT_FAILURE);
      }

      if (-1 != msgrcv(msqid, &message2, sizeof(struct msgbuff),
                       PRIO_CLIENT_TO_SERVER, 0)) {
        char name[NAME_LEN];
        char format[NAME_LEN];
        sprintf(format, "%%%ds %%u", NAME_LEN - 1);
        unsigned scanned_id;
        if (sscanf(message2.text_msg, format, name, &scanned_id) == 2) {
          num_clients++;
          message2.prioritet = 1;
          res_msgsnd = msgsnd(msqid_inner, &message2, sizeof(message1), 0);
          if (res_msgsnd == -1) {
            perror("sending message 2");
            exit(EXIT_FAILURE);
          }
        }
      }
    }
  } else {
    pid_t process_stop_pid = fork();
    if (0 == process_stop_pid) { /* процесс для выхода из программы
        exit when pushed SYMBOL_EXIT and ENTER
        при нажатии клавиши и ввода этот процесс передает через очередь
        сообщений слово exit и главный процесс завершает работу
        */
      char chr = ' ';
      while (chr != SYMBOL_EXIT) {
        chr = getchar();
      }
      sprintf(message1.text_msg, "exit 0");
      message1.prioritet = 1;
      msgsnd(msqid_inner, &message1, sizeof(message1), IPC_NOWAIT);
    } else { /* главный процесс  */
      Information info = {0};
      info.size_chat = info.size_clients = 0;
      struct chat_msg chatmsg;
      while (1) {
        if (-1 != msgrcv(msqid_inner, &message2, sizeof(struct msgbuff), 1,
                         IPC_NOWAIT)) {
          char name[NAME_LEN];
          char format[NAME_LEN];
          sprintf(format, "%%%ds %%u", NAME_LEN - 1);
          unsigned scanned_id;
          if (sscanf(message2.text_msg, format, name, &scanned_id) == 2) {
            if (0 == scanned_id && 0 == strncmp(name, "exit", 5)) {
              break;  // EXIT PROGRAMM
            } else {  // при первом контакте клиента его имя и ID сохраняются в
                      // базе данных и  для связи сервера и клиента создается
                      // индивидуальная очередь сообщений
              strncpy(info.clients[info.size_clients].name, name, NAME_LEN);
              info.clients[info.size_clients].id = scanned_id;
              if ((info.clients[info.size_clients].mqueue =
                       make_msqid(NUMBER_TO_KEY + scanned_id)) < 0) {
                perror("make_msqid individual");
                exit(EXIT_FAILURE);
              }
              // при первом контакте сервер передает клиенту список
              // имен всех старых клиентов
              // а имя нового клиента - старым
              sprintf(message2.text_msg, "%c%s", SYMBOL_NEW_NAME, name);
              message2.prioritet = PRIO_SERVER_TO_CLIENT;
              for (unsigned i = 0; i < info.size_clients; i++) {
                sprintf(message1.text_msg, "%c%s", SYMBOL_NEW_NAME,
                        info.clients[i].name);
                message1.prioritet = PRIO_SERVER_TO_CLIENT;
                msgsnd(info.clients[info.size_clients].mqueue, &message1,
                       sizeof(message1), IPC_NOWAIT);
                msgsnd(info.clients[i].mqueue, &message2, sizeof(message2),
                       IPC_NOWAIT);
              }

              for (unsigned i = 0; i < info.size_chat; i++) {
                chatmsg.prioritet = PRIO_SERVER_TO_CLIENT;
                sprintf(chatmsg.text_msg, "%c%s", SYMBOL_CHAT, info.chat[i]);
                msgsnd(info.clients[info.size_clients].mqueue, &chatmsg,
                       sizeof(chatmsg), IPC_NOWAIT);
              }
              info.size_clients++;
            }
          }
        }
        struct msgbuff msg = {0};
        for (unsigned i = 0; i < info.size_clients;
             i++) {  // сканируются все индивидуальные очереди сообщений с
                     // клиентами и считываются сообщения от клиентов
          if (-1 != msgrcv(info.clients[i].mqueue, &msg, sizeof(struct msgbuff),
                           PRIO_CLIENT_TO_SERVER, IPC_NOWAIT)) {
            sprintf(info.chat[info.size_chat], "%s: %s", info.clients[i].name,
                    msg.text_msg);
            chatmsg.prioritet = PRIO_SERVER_TO_CLIENT;
            sprintf(chatmsg.text_msg, "%c%s", SYMBOL_CHAT,
                    info.chat[info.size_chat]);
            for (unsigned j = 0; j < info.size_clients;
                 j++) {  // полученное сообщение отправляется от сервера всем
                         // клиентам
              msgsnd(info.clients[j].mqueue, &chatmsg, sizeof(msg), IPC_NOWAIT);
            }
            info.size_chat++;
          }
        }
        sleep(3);
      }
      struct msqid_ds buf;
      for (unsigned i = 0; i < info.size_clients; i++) {
        msgctl(info.clients[i].mqueue, IPC_RMID, &buf);
        printf("REMOVING QUEUE FOR CLIENT #%u\n", i);
      }
      msgctl(msqid, IPC_RMID, &buf);
      msgctl(msqid_inner, IPC_RMID, &buf);
      kill(process_stop_pid, SIGTERM);
    }
  }
  printf("SERVER PROCESS #%d ENDS\n", getpid());
  exit(EXIT_SUCCESS);
}
