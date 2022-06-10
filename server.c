// program serwera

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio_ext.h>

#define BUFF_MAX 100 // maksymalna ilość znaków w komunikacie
key_t key = 65464; // klucz używany do stworzenia kolejki komunikatów

char replaceFrom[4] = "pies"; // sekwencja zamieniana
char replaceInto[4] = "hau!"; // sekwencja wstawiana

// struktura opisująca wiadomość kolejki komunikatów
struct Msg {
    long mtype;
    char mtext[BUFF_MAX];
};

// funkcja zamieniająca
void replaceString(struct Msg * msg) {
	int repeatChars = 0; // zmienna pomocnicza inkrementująca wystąpienia znaków do zamiany
	int length = sizeof(replaceInto) / sizeof(*replaceInto); // długość sekwencji
	// sprawdź, czy długość obu sekwencji jest taka sama, jeśli nie to zakończ program
	if (sizeof(replaceFrom) != sizeof(replaceInto)) {
		printf("Obie sekwencje musza miec taka sama dlugosc");
		exit(1);
	}
	for(int i = 0; i < BUFF_MAX; i++) {
		// jeśli jest to kolejny znak z sekwencji zamienianej, zinkrementuj pozycję
		if (msg->mtext[i] == replaceFrom[repeatChars]) {
			repeatChars++;
		} else { // jeśli nie jest to znak zamieniający, przypisz zero
			repeatChars = 0;
		}
		// jeśli znaleziono wszystkie znaki do zamiany, zamień
		if (repeatChars == length) {
			for(int j = length; j > 0; j--) {
				msg->mtext[i - j + 1] = replaceInto[length - j];
			}
			repeatChars = 0; // wyczyść bufor znaków
		}
	}
}

void main(void) {
    	struct Msg msg;
    	int msId = msgget(key, IPC_CREAT|0600); // tworzenie kolejki

	// jeśli nie udało się utworzyć kolejki, zakończ program
    	if(msId == -1) {
        	printf("Nie udalo sie utworzyc kolejki");
        	exit(1);
    	}

	printf("=== Serwer slucha komunikatow od klienta z ID kolejki: %i ===\n", key);
    	// nasłuchuj wiadomości przychodzących do kolejki od klienta
	while((msgrcv(msId, &msg, BUFF_MAX, 1, 0)) != 0) {
        	printf("Komunikat przed zmiana: %s", msg.mtext);
		msg.mtype=2;
		// zamiana wszystkich wielkich liter na małe na podstawie tabeli ASCII
		for(int i = 0; i < BUFF_MAX; i++) {
			if(msg.mtext[i] >= 65 && msg.mtext[i] <= 90) {
				msg.mtext[i] += 32; // do znaku dodawanie 32, bo jest to różnica między wielką a małą literą
			}
		}
		// zamiana wszystkich ciągów znaków 'pies' na 'hau!'
		replaceString(&msg);

		printf("Komunikat przetworzony przez serwer: %s\n", msg.mtext);
    		// wyślij przetworzony komunikat do klienta
		if(msgsnd(msId, &msg, BUFF_MAX, 0) != -1) {
        		printf("Wyslano komunikat do klienta.");
    		} else {
        		printf("Nie udalo sie wyslac komunikatu.");
        		exit(1);
    		}
		__fpurge(stdin); // wyczyść bufor wejściowy
	}
}
