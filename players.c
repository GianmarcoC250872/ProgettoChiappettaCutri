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

void inserisciGiocatoreInTesta(PlayerList *list, char *name, int pos, int id) {
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
    new_player->next = list->head;
    list->head = new_player;
}

void inserisciGiocatoreOrdinato(PlayerList *list, char *name, int pos, int id) {
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
    new_player->next = NULL;

    if (listaGiocatoriVuota(list) || pos > list->head->position) {
        new_player->next = list->head;
        list->head = new_player;
    } else {
        Player *current = list->head;
        while (current->next != NULL && current->next->position >= pos) {
            current = current->next;
        }
        new_player->next = current->next;
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

bool listaGiocatoriVuota(PlayerList *list) {
    return list == NULL || list->head == NULL;
}

Player *cercaGiocatore(PlayerList *list, char *name) {
    Player *current = list->head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

void rewindGiocatori(PlayerList *list) {
    if (list)
        list->cursor = list->head;
}

bool hasNextGiocatore(PlayerList *list) {
    return list && list->cursor != NULL;
}

Player *nextGiocatore(PlayerList *list) {
    if (!list || !list->cursor)
        return NULL;

    Player *current = list->cursor;
    list->cursor = list->cursor->next;
    return current;
}

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
