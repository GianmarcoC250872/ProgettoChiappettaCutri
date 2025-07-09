#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "players.h"
#include "mosse.h"
#include "square_types.h"
#include "tavolaGioco.h"
#include "config.h"

void stampaTitolo() {
    printf("\n===============================\n");
    printf("|        GIOCO DELL'OCA       |\n");
    printf("===============================\n\n");
}

int leggiNumero(const char* prompt, int min, int max) {
    int val;
    char buffer[100];
    do {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL && sscanf(buffer, "%d", &val) == 1) {
            if (val >= min && val <= max) return val;
        }
        printf("Input non valido. Riprova.\n");
    } while (1);
}

int scegliColoreDisponibile(int colori_assegnati[], int num_giocatori) {
    const char* nomi_colori[4] = {"Rosso", "Verde", "Blu", "Giallo"};

    while (1) {
        printf("**SCEGLI UN COLORE**: ");
        for (int i = 0; i < 4; i++) {
            if (!colori_assegnati[i]) {
                printf("%d. %s ", i + 1, nomi_colori[i]);
            }
        }

        int scelta = leggiNumero("\nScelta: ", 1, 4);
        if (!colori_assegnati[scelta - 1]) {
            colori_assegnati[scelta - 1] = 1;
            return scelta - 1;
        } else {
            printf("Colore gia' scelto da un altro giocatore. Riprova.\n");
        }
    }
}

void attendiInvio() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF); //svuota il buffer
    printf("Premi INVIO per tirare i dadi...");
    while (getchar() != '\n'); //ferma il programma finchè non legge invio
}

int nomeGiaPresente(PlayerList *lista, const char *nome) {
    Player *curr = lista->head;
    while (curr != NULL) {
        if (strcmp(curr->name, nome) == 0) {
            return 1; //nome già usato
        }
        curr = curr->next;
    }
    return 0;
}

void chiediNomiGiocatori(PlayerList *lista, int num_giocatori) {
    char nome[MAX_NAME_LEN];
    int colori_assegnati[4] = {0};

    for (int i = 0; i < num_giocatori; i++) {
        do {
            printf("Inserisci il nome del giocatore %d: ", i + 1);
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = '\0'; //sostituisce il \n con il terminatore di stringa(per la strcmp dopo)

            if (nomeGiaPresente(lista, nome))
                printf("ATTENZIONE! Nome gia' usato! Inserisci un nome diverso.\n");
            else
                break;
        } while (1);

        int colore_id = scegliColoreDisponibile(colori_assegnati, num_giocatori);
        inserisciGiocatoreOrdinato(lista, nome, 0, i + 1, colore_id);
    }
}

ModalitaGioco scegliModalita() {
    int scelta = leggiNumero("Scegli la modalita di gioco:\n1. Classica (fissa)\n2. Arcade (caselle mischiate)\nScelta: ", 1, 2);
    if (scelta == 1)
        return FISSA;
    else
        return ARCADE;
    }

int main() {

    srand(time(NULL));
    FILE *logfile = NULL; //inizializzazione file storico
    int rigioca = 1;
    int dado1, dado2, d1, d2 = 0;


    while (rigioca) {
        stampaTitolo();

        //apertura del file in append
        logfile = fopen("storico_partite.txt", "a");
        if (!logfile) {
            perror("Errore apertura storico_partite.txt");
            exit(1);
        }

        /* ====LOG INIZIO PARTITA==== */
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        fprintf(logfile, "\n==============================\n");
        fprintf(logfile, "Partita iniziata il %02d/%02d/%04d alle %02d:%02d:%02d\n",
                t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
                t->tm_hour, t->tm_min, t->tm_sec);

        int num_giocatori = leggiNumero("Inserisci il numero di giocatori (2-4): ", 2, 4);
        PlayerList *giocatori = creaListaGiocatori();
        ListaMosse *storico = creaListaMosse();
        chiediNomiGiocatori(giocatori, num_giocatori);

        ModalitaGioco modalita = scegliModalita();

        /* ====LOG MODALITA'==== */
        if (modalita == FISSA)
            fprintf(logfile, "Modalità: Classica\n");
        else
            fprintf(logfile, "Modalità: Arcade\n");

        printf("Perfetto, stampo la tavola di gioco...\n");
        sleep(1);
        inizializzaTavola(modalita);
        stampaTavola(giocatori, num_giocatori);


        int turno = 1;
        int vincitore_trovato = 0;

        while (!vincitore_trovato) {
            printf("\nINIZIO TURNO %d\n", turno);
            rewindGiocatori(giocatori);

            while (hasNextGiocatore(giocatori)) {
                Player *g = nextGiocatore(giocatori);
                printf("Turno di %s\n", g->name);

                if (g->stato == ARRIVATO) continue;
                if (g->stato == BLOCCATO) {
                    printf("%s e' bloccato/a e salta il turno.\n", g->name);
                    g->stato = LIBERO;
                    continue;
                }
                if (g->stato == NEL_POZZO) {
                    g->turniNelPozzo++;
                    if (g->turniNelPozzo > 3) {
                        printf("%s e' stato/a nel pozzo per 3 turni, ora esce automaticamente!\n", g->name);
                        g->stato = LIBERO;
                        g->turniNelPozzo = 0;
                    } else {
                        printf("ATTENZIONE: %s, sei nel POZZO! Per uscire devi fare DOPPIO.\n", g->name);
                        attendiInvio();

                        d1 = rand() % 6 + 1;
                        d2 = rand() % 6 + 1;

                        printf("%s ha tirato: %d + %d\n", g->name, d1, d2);
                        if (d1 == d2) {
                            printf("DOPPIO! %s, esci dal POZZO!\n", g->name);
                            g->stato = LIBERO;
                            g->turniNelPozzo = 0;
                        }
                        else
                            printf("Niente DOPPIO... %s, resti nel pozzo.\n", g->name);
                        continue;
                    }
                }

                attendiInvio();
                dado1 = rand() % 6 + 1;
                dado2 = rand() % 6 + 1;
                int somma = dado1 + dado2;

                int pos_iniziale = g->position;
                g->position += somma;

                printf("%s ha tirato %d + %d = %d --> da casella %d a %d\n",
                       g->name, dado1, dado2, somma, pos_iniziale, g->position);

                stampaTavola(giocatori, num_giocatori);

                //controllo casella e assegnazione effetto al giocatore
                Casella *casella = getCasella(g->position);
                TipoCasella tipo;

                if (casella != NULL)
                    tipo = casella->tipo;
                else
                    tipo = NORMALE;

                if (tipo != NORMALE) {
                    printf("%s, sei finito/a su una casella speciale: %s\n", g->name, nomeTipoCasella(tipo));

                    if (tipo == AVANZA) {
                        printf("Ti sposti in AVANTI di %d casella/e!\n", casella->effetto);
                        g->position += casella->effetto;
                        sleep(1);
                        stampaTavola(giocatori, num_giocatori);
                    }
                    else if (tipo == TORNA_INDIETRO) {
                        printf("Ti sposti INDIETRO di %d casella/e!\n", casella->effetto);
                        g->position -= casella->effetto;
                        sleep(1);
                        stampaTavola(giocatori, num_giocatori);
                        if (g->position < 0) g->position = 0;
                    }
                    else if (tipo == BACK_TO_START) {
                        g->position = 0;
                        sleep(1);
                        stampaTavola(giocatori, num_giocatori);
                    }
                    else if (tipo == STOP) {
                        g->stato = BLOCCATO;
                    }
                    else if (tipo == POZZO) {
                        Player *altro = giocatori->head;
                        int sostituito = 0;
                        while (altro != NULL) {
                            if (altro != g && altro->position == g->position && altro->stato == NEL_POZZO) {
                                printf("%s libera %s dal pozzo!\n", g->name, altro->name);
                                altro->stato = LIBERO;
                                altro->turniNelPozzo = 0;
                                g->stato = NEL_POZZO;
                                g->turniNelPozzo = 0;
                                sostituito = 1;
                                break;
                            }
                            altro = altro->next;
                        }
                        if (!sostituito) {
                            g->stato = NEL_POZZO;
                            g->turniNelPozzo = 0;
                        }
                    }
                }

                if (g->position >= MAX_SQUARES - 1) {
                    g->position = MAX_SQUARES;
                    g->stato = ARRIVATO;
                    printf("\n%s e' arrivato/a alla fine!\n", g->name);
                    vincitore_trovato = 1;
                }

                Mossa m = creaMossa(turno, g->id, dado1, dado2, pos_iniziale, g->position, tipo);
                inserisciMossaInTesta(storico, m);

                /* ====LOG MOSSE==== */
                fprintf(logfile, "Turno %d | G%d (%s): %d + %d | da %d a %d | %s\n", turno, g->id, g->name,
                        dado1, dado2, pos_iniziale, g->position, nomeTipoCasella(tipo));

                if (vincitore_trovato) break;
            }

            printf("FINE TURNO %d\n", turno);
            turno++;
        }

        /* ====LOG CLASSIFICA==== */
        stampaClassifica(giocatori, logfile); //stampa su file implementata direttamente nella funzione di stampa della classifica


        rigioca = leggiNumero("\nVuoi giocare un'altra partita? (1 = si, 0 = no): ", 0, 1);
        liberaListaGiocatori(giocatori);
        freeListaMosse(storico);
        fclose(logfile);
    }

    return 0;
}
