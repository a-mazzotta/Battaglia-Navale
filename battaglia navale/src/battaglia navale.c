/*
 ============================================================================
 Name        : battaglia.c
 Author      : Alessandro Mazzotta
 Version     : 1.0
 Description : Variante della battaglia navale con una randomizzazione della posizione di 6 navi di dimensione 1
 	 	 	   su un tavolo 6x6 con 10 tentativi.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#define N_NAVI 6 //massimo RIGHE*COLONNE
#define N_TENTATIVI 10
#define PUNTI_VITT 4 //punti minimi per la vittoria
#define RIGHE 6 //massimo 10
#define COLONNE 6 //massimo 28
#define LETTERA 64 //codice ascii per le lettere del tavolo
#define NUMERO 48 //codice ascii per il numero del tavolo

char **calcola_dimensione();
char **crea_tavolo(char **tavolo);
char **randomizza_navi(char **tavolo);
void stampa_tavolo(char **tavolo);
int *scelta_campo();
int verifica_campo(int *posizione, char **tavolo_navi, char **tavolo_giocatore);
void verifica_risultato(char **tavolo_bot, char **tavolo_giocatore, int tentativi, int punti);

/*
 * Il main gestisce tutte le funzioni riguardanti il gioco tenendo conto
 * dei tentivi, dei punti e dell'esito finale
 */
int main() {
	char scelta[7], **tavolo_bot, **tavolo_giocatore;
	int i, *posizione, esito, tentativi, punti;

	tavolo_bot = calcola_dimensione();
	tavolo_giocatore = calcola_dimensione();

	srand(time(NULL));

	do {
		//titolo e menu'
		system("cls");

		printf("########     ###    ######## ######## ##       ########  ######  ##     ## #### ########  \n"
				"##     ##   ## ##      ##       ##    ##       ##       ##    ## ##     ##  ##  ##     ## \n"
				"##     ##  ##   ##     ##       ##    ##       ##       ##       ##     ##  ##  ##     ## \n"
				"########  ##     ##    ##       ##    ##       ######    ######  #########  ##  ########  \n"
				"##     ## #########    ##       ##    ##       ##             ## ##     ##  ##  ##        \n"
				"##     ## ##     ##    ##       ##    ##       ##       ##    ## ##     ##  ##  ##        \n"
				"########  ##     ##    ##       ##    ######## ########  ######  ##     ## #### ##        \n");

		printf("\nBenvenuto, questo programma ti permette di giocare ad una variante di battaglia navale\n"
				"dove il computer posizionera' 6 navi di dimensione 1 in una tavola di gioco.\n"
				"Il tuo obbiettivo e' di affondarne almeno 4 con 10 tentativi.\n"
				"Buona fortuna!\n"
				"Scrivi 'Start' per iniziare oppure 'Exit' per uscire.\n -->  ");
		scanf("%s", scelta);

		for(i = 0; i < strlen(scelta); i++) {
			scelta[i] = toupper(scelta[i]);
		}

		//programmazione difensiva
		if((strcmp(scelta, "START") != 0) && (strcmp(scelta, "EXIT") != 0)) {
			printf("\nScelta non valida!\n");
			system("pause");
		}
		//inizio gioco
		else if(strcmp(scelta, "START") == 0) {

			tavolo_giocatore = crea_tavolo(tavolo_giocatore);
			tavolo_bot = crea_tavolo(tavolo_bot);

			tavolo_bot = randomizza_navi(tavolo_bot);
			tentativi = N_TENTATIVI;
			punti = 0;

			do {
				system("cls");

				printf("Tentativi rimanenti: %d\tPunti: %d\n\n", tentativi, punti);
				stampa_tavolo(tavolo_giocatore);

				posizione = scelta_campo();

				esito = verifica_campo(posizione, tavolo_bot, tavolo_giocatore);
				if(esito == -1) {
					printf("\n\nCampo gia' colpito!\n");
				} else if(esito == 0) {
					printf("\n\nHai fatto un buco nell'acqua!\n");
					tentativi--;
				} else if(esito == 1) {
					printf("\n\nColpito e affondato!\n");
					tentativi--;
					punti++;
				}

				system("pause");

				//se i tentativi finiscono o se vengono distrutte tutte le navi
			} while((tentativi > 0) && (punti < N_NAVI));

			verifica_risultato(tavolo_bot, tavolo_giocatore, tentativi, punti);
		}


	} while (strcmp(scelta, "EXIT") != 0); //uscita nel caso la scelta e' exit

	printf("\nSpero ti sia divertito =D\n");
	system("pause");

	return EXIT_SUCCESS;
}

/*
 * Calcola e restituisce la matrice con la sua dimensione
 */
char **calcola_dimensione() {
	char **tavolo = (char**)malloc(RIGHE * sizeof(char*)); //calcolo la dimensione per l'array
	int i;

	//calcolo la dimensione della matrici
	for(i = 0; i < RIGHE; i++) {
		tavolo[i] = (char*) malloc(COLONNE * sizeof(char*));
	}
	return tavolo;
}

/*
 * funzione che crea automaticamente un tavolo di gioco nominando
 * le colonne con i numeri e le righe con lettere tramite i rispettivi codici ascii
 * e restituisce il tavolo creato
 */
char **crea_tavolo(char **tavolo) {
	int i,j;

	for(i = 0; i < RIGHE; i++) {
		for(j = 0; j < COLONNE; j++) {

			if((i == 0) && (j == 0)) {
				tavolo[i][j] = (char)47; //converte il numero 47 nel rispettivo ASCII /
			} else if(i == 0) {
				tavolo[i][j] = (char)NUMERO+j; //converte l'ASCII partendo dallo 0
			} else if(j == 0) {
				tavolo[i][j] = (char)LETTERA+i; //converte l'ASCII partendo dalla @ (il codice dopo e' la A)
			} else {
				tavolo[i][j] = (char)126; //coonverte il numero 126 nel rispettivo ASCII ~
			}
		}
	}

 return tavolo;
}

/*
 * funzione che crea dei valori randomici da usare come indici per il posizionamento delle navi
 * restituendo il tavolo con le navi inserite
 */
char **randomizza_navi(char **tavolo) {
	int k, i, j;

	for(k = 0; k < N_NAVI; k++) {
		i = rand() % RIGHE;
		j = rand() % COLONNE;
		if(tavolo[i][j] == (char)126) { //codice ascii per la ~, controlla che in quella posizione non ci sia già una nave
			tavolo[i][j] = (char)NUMERO; //codice ascii dello 0
		}
		else {
			k--;
		}
	}


	return tavolo;
}

/*
 * procedura per la stampa del tavolo passato in input
 */
void stampa_tavolo(char **tavolo) {
	int i, j;

	for(i = 0; i<RIGHE; i++) {
		for(j = 0; j<COLONNE; j++) {
			printf("[%c]", tavolo[i][j]);
		}
		printf("\n");
	}
}

/*
 * funzione che permette all'utente di inserire i valori che verranno utilizzati per il bombardamento
 * restituendo gli indici corrispettivi al campo scelto dall'utente
 */
int *scelta_campo() {
	int *pos = malloc(sizeof(char));
	char posizione[1];
	int flag = 0, i, j;

	do {
		i = 0;
		j = 0;

		while(getchar() != '\n');
		printf("\nSeleziona il campo da colpire! Es. A1\n-->  ");
		scanf("%2s", posizione);

		posizione[0] = toupper(posizione[0]);

		while(posizione[0] != LETTERA+i && i<RIGHE-1) { //fin quando non finiscono le righe, cerca la lettera scelta, nel tavolo
			i++;
			if(posizione[0] == LETTERA+i) { //se la lettera esiste
				while(posizione[1] != NUMERO+j && j<COLONNE-1) { //fin quando non finiscono le colonne, cerca il numero scelto, nel tavolo
					j++;
					if(posizione[1] == NUMERO+j) { //se il numero esiste
						flag = 1;
					}
				}
			}
		}

		if(flag == 0) {
			printf("\nCampo non esistente!\n");
			system("pause");
		}

	} while(flag == 0);

	pos[0] = i;
	pos[1] = j;

	return pos;
}


/*
 * funzione che verifica se il bombardamento ha avuto successo o no, oppure se quel campo e' stato gia' bombardato
 * restituendo l'esito del bombardamento
 */
int verifica_campo(int *posizione, char **tavolo_navi, char **tavolo_giocatore) {
	int esito = 0, riga, colonna;

	riga = posizione[0];
	colonna = posizione[1];

	if(tavolo_giocatore[riga][colonna] == (char)88 || tavolo_giocatore[riga][colonna] == (char)79) { //se gia' stato colpito
		esito = -1;
	} else if(tavolo_navi[riga][colonna] == (char)48) { //se e' presente una nave
		tavolo_giocatore[riga][colonna] = (char)88; //X
		esito = 1;
	} else if(tavolo_navi[riga][colonna] == (char)126) { //se non e' presente una nave
		tavolo_giocatore[riga][colonna] = (char)79; //O
		esito = 0;
	}

	return esito;
}

/*
 * Procedura che stampa la posizione delle navi sul tavolo e verifica
 * se i punti minimi sono stati superati o no,
 * nel caso siano stati superati, stampa un messaggio di vittoria,
 * nel caso non siano stati superati, stampa un messaggio di sconfitta.
 */
void verifica_risultato(char **tavolo_bot, char **tavolo_giocatore, int tentativi, int punti) {
	system("cls");

	//poiche' quando i tentativi finiscono, il numero viene decrementato ugualmente,
	//il numero dei tentativi risulta -1, per questo lo incrementiamo di 1 per stampare 0
	if(tentativi < 0) {
		tentativi++;
	}
	printf("Tentativi rimanenti: %d\tPunti: %d\n", tentativi, punti);
	printf("\nEcco la posizione delle navi!\n");
	stampa_tavolo(tavolo_bot);
	printf("\n\nCampi bombardati:\n\n");
	stampa_tavolo(tavolo_giocatore);

	system("timeout 2 > NUL");

	if(punti >= PUNTI_VITT) {
		printf("\n\n:::     ::: ::::::::::: ::::::::::: :::::::::::  ::::::::  :::::::::  :::::::::::     :::     \n"
				":+:     :+:     :+:         :+:         :+:     :+:    :+: :+:    :+:     :+:       :+: :+:   \n"
				"+:+     +:+     +:+         +:+         +:+     +:+    +:+ +:+    +:+     +:+      +:+   +:+  \n"
				"+#+     +:+     +#+         +#+         +#+     +#+    +:+ +#++:++#:      +#+     +#++:++#++: \n"
				" +#+   +#+      +#+         +#+         +#+     +#+    +#+ +#+    +#+     +#+     +#+     +#+ \n"
				"  #+#+#+#       #+#         #+#         #+#     #+#    #+# #+#    #+#     #+#     #+#     #+# \n"
				"    ###     ###########     ###         ###      ########  ###    ### ########### ###     ### \n\n");
	} else if(punti < PUNTI_VITT) {
		printf("\n\n  #####    #####   #######  #     #  #######  ###  #######  #######     #    \n"
				" #     #  #     #  #     #  ##    #  #         #      #        #       # #   \n"
				" #        #        #     #  # #   #  #         #      #        #      #   #  \n"
				"  #####   #        #     #  #  #  #  #####     #      #        #     #     # \n"
				"       #  #        #     #  #   # #  #         #      #        #     ####### \n"
				" #     #  #     #  #     #  #    ##  #         #      #        #     #     # \n"
				"  #####    #####   #######  #     #  #        ###     #        #     #     # \n\n");
	}

	system("pause");
}

