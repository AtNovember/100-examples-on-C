/* listing 68a, 100 primerov C */

#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// updated:
// люблй сервер должен уметь прерываться/перезапускаться
// при поступлении сигналов, напр SIG_HUP
//#include "sock.h"
//#include <signal.h>

/* обработчик сигнала SIGPIPE */
//sigpipe_handler() {
//    err_quit("Получен SIGPIPE \n");
//}


/*
 * Определяем макросы:
 *  SERVER_PORT Нормер порта сервера
 *  BUF_SIZE размер буфера передаваемых данных
 *  MSG_TO_SEND запрос клиенту
 * */
#define SERVER_PORT 1234
#define BUF_SIZE 64
#define MSG_TO_SEND "Version\n"

int main() {

    // updated:
    //    signal(SIGPIPE, sigpipe_handler);

    /*
     * определяем 2 сокета:
     * 1й это сокет сервера
     * а 2й для обмена данными с клиентом
     * */
    int sock1, sock2;

    /*
     * ans_len - используется для хранения размера передаваемой клиентом информации -
     * фактический размер структуры struct sockaddr_in
     *
     * total - это общий счетчик числа клиентов.
     * Используется для вывода порядкового номера клиента
     *
     * */
    int ans_len, total=0;

    /* это буфер обмена информацией */
    char buffer[BUF_SIZE];

    /*
     * создаем две структуры типа sockaddr_in
     * одна для сервера sin, другая для клиента client
     */
    struct sockaddr_in sin, client;


    /* создаем "серверный" сокет. Набор протоколов TCP/IP */
    sock1 = socket(AF_INET, SOCK_STREAM, 0);

    /* инициализируем структуру sin */
    memset((char *)&sin, '\0', sizeof(sin));

    sin.sin_family = AF_INET;  // TCP/IP
    sin.sin_addr.s_addr = INADDR_ANY; // можем работать на любом адресе
    sin.sin_port = SERVER_PORT; // указываем порт

    /* связываем сокет с адресом и портом сервера */
    bind(sock1, (struct sockaddr *)&sin, sizeof(sin));

    printf("Server is running... \n");

    /*
     * прослушиваем сокет 1 и максимальное количество клиентов
     * */
    listen(sock1, 3);

    // как и любой нормальный сервер,
    // мы должны работать в бесконечном цикле,
    // постоянно обрабатывая запросы клиентов
    while(1) {
        ans_len = sizeof(client);
        sock2 = accept(sock1, &client, &ans_len);
        write(sock2, MSG_TO_SEND, sizeof (MSG_TO_SEND));
        total +=1;
        printf("Client no %d\n", total);
        shutdown(sock2, 0);
        close(sock2);
    }

    return 0;
}
