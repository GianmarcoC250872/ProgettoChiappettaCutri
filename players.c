#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "players.h"

PlayerList* creaListaGiocatori() {
    PlayerList *list = (PlayerList *)malloc(sizeof(PlayerList));
    if (list != NULL) {
        list->head = NULL;
        list->cursor = NULL;
    }
    return list;
}

void inserisciGiocatoreInTesta(PlayerList *list, char *name, int pos, int id, int colore_id) {
    Player *new_player = (Player *)malloc(sizeof(Player));
    if (new_player == NULL) {
        printf("Errore allocazione memoria\n");
        return;
    }
    new_player->id = id;
    strncpy(new_player->name, name, MAX_NAME_LEN);
    new_player->position = pos;
    new_player->stato = LIBERO;
    new_player->turniNelPozzo = 0;
    new_player->colore_id = colore_id;
    new_player->next = list->head;
    list->head = new_player;
}


void inserisciGiocatoreOrdinato(PlayerList *list, char *name, int pos, int id, int colore_id) {
    Player *new_player = (Player *)malloc(sizeof(Player));
    if (new_player == NULL) {
        printf("Errore allocazione memoria\n");
        return;
    }

    new_player->id = id;
    strncpy(new_player->name, name, MAX_NAME_LEN);
    new_player->position = pos;
    new_player->stato = LIBERO;
    new_player->turniNelPozzo = 0;
    new_player->colore_id = colore_id;
    new_player->next = NULL;

    if (listaGiocatoriVuota(list) || pos > list->head->position) {
        new_player->next = list->head;
        list->head = new_player;
    } else {
        Player *current = list->head;
        while (current->next != NULL && current->next->position >= pos) {
            current = current->next;
        }
        new_player->next = current->next;   //inserimento di new_player tra current e current->next
        current->next = new_player;
    }
}


void stampaListaGiocatori(PlayerList *list) {
    if (!list) return;
    Player *current = list->head;
    printf("----- GIOCATORI -----\n");
    while (current != NULL) {
        printf("ID:%d | %s - Posizione: %d", current->id, current->name, current->position);
        switch (current->stato) {
            case LIBERO:
                break;
            case BLOCCATO:
                printf(" (Salta 1 turno)");
                break;
            case NEL_POZZO:
                printf(" (Nel pozzo, turno %d)", current->turniNelPozzo);
                break;
            case ARRIVATO:
                printf(" (Arrivato)");
                break;
        }
        printf("\n");
        current = current->next;
    }
    printf("---------------------\n");
}

int lunghezzaListaGiocatori(PlayerList *list) {
    int count = 0;
    Player *current = list->head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}


//restituisce true, quindi la lista è vuota, se la lista non esiste (NULL), oppure se il puntatore alla testa della lista è null, quindi la lista esiste ma non vi sono elementi all'interno
bool listaGiocatoriVuota(PlayerList *list) {
    return list == NULL || list->head == NULL;
}


//restituisce il puntatore il puntatore al giocatore che stiamo cercando (attravreso la comparazione del name) o eventualmente NULL
Player *cercaGiocatore(PlayerList *list, char *name) {
    Player *current = list->head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}


//inizializza il cursore
void rewindGiocatori(PlayerList *list) {
    if (list)
        list->cursor = list->head;
}


//restituisce true se c'è almeno un altro giocatore da "leggere"
bool hasNextGiocatore(PlayerList *list) {
    return list && list->cursor != NULL;
}


//restiituisce NULL se la lista o il cursore non esistono, altrimenti salva il giocatore corrente, sposta il cursore al giocatore successivo e ritorna il giocatore che si stava leggendo
Player *nextGiocatore(PlayerList *list) {
    if (!list || !list->cursor)
        return NULL;

    Player *current = list->cursor;
    list->cursor = list->cursor->next;
    return current;
}


//se la lista esiste: scorre tutta la lista; licera ad uno ad uno i nodi Players; alla fine libera la struttura PlayerList stessa
void liberaListaGiocatori(PlayerList *list) {
    if (!list) return;
    Player *current = list->head;
    while (current != NULL) {
        Player *tmp = current;
        current = current->next;
        free(tmp);
    }
    free(list);
}

void stampaClassifica(PlayerList *lista, FILE *logfile) {
    int n = 0;
    Player *curr = lista->head;
    Player *giocatori[MAX_PLAYERS];

    // Copia i puntatori dei giocatori in un array per ordinamento
    while (curr != NULL && n < MAX_PLAYERS) {
        giocatori[n++] = curr;
        curr = curr->next;
    }

    // Ordina per posizione decrescente (pseudo selection sort)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (giocatori[i]->position < giocatori[j]->position) {
                Player *temp = giocatori[i];
                giocatori[i] = giocatori[j];
                giocatori[j] = temp;
            }
        }
    }

    fprintf(stdout, "\n==== CLASSIFICA FINALE ====\n");
    fprintf(logfile, "\n==== CLASSIFICA FINALE ====\n");

    // Stampa la classifica sia su terminale che su file
    for (int i = 0; i < n; i++) {
        if (giocatori[i]->stato == ARRIVATO) {
            fprintf(stdout, "%d. %s - ARRIVATO\n", i + 1, giocatori[i]->name);
            fprintf(logfile, "%d. %s - ARRIVATO\n", i + 1, giocatori[i]->name);
        } else {
            fprintf(stdout, "%d. %s - casella %d\n", i + 1, giocatori[i]->name, giocatori[i]->position);
            fprintf(logfile, "%d. %s - casella %d\n", i + 1, giocatori[i]->name, giocatori[i]->position);
        }
    }
}

