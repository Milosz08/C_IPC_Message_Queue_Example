// program klienta

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio_ext.h>

#define BUFF_MAX 100 // maksymalna ilość znaków w komunikacie
key_t key = 65464; // klucz używany do stworzenia kolejki komunikatów

// struktura opisująca wiadomość kolejki komunikatów
struct Msg {
    long mtype;
    char mtext[BUFF_MAX];
};

void main(void) {
    struct Msg msg;
    int msId = msgget(key, IPC_CREAT|0600); // utworzenie kolejki

    // jeśli nie uda się utworzyć kolejki, zakończ program
    if(msId == -1) {
        printf("Nie udalo sie utworzyc kolejki");
        exit(1);
    }

    printf("Jaki komunikat chcesz wprowadzić: ");
    __fpurge(stdin); // czyszczenie bufora wejściowego
    // pobranie znaków z bufora strumienia wejściowego
    fgets(msg.mtext, BUFF_MAX, stdin);
    msg.mtype=1;

    // sprawdzenie i wysłanie komunikatu, jeśli błąd zakończ działanie programu
    if(msgsnd(msId, &msg, BUFF_MAX, 0) == -1) {
        printf("Nie udalo sie wyslac komunikatu");
        exit(1);
    }

    // odebranie wiadomości z serwera (z parametrem )
    if(msgrcv(msId, &msg, BUFF_MAX, 2, 0) != 0) {
        printf("Komunikat przetworzony przez serwer: %s\n", msg.mtext);
    } else {
        printf("Nie udalo sie odebrac komunikatu");
        exit(1);
    }
}