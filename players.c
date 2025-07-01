#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "players.h"

PlayerList createPlayerList() {
    PlayerList list;
    list.head = NULL;
    list.cursor = NULL;
    return list;
}

bool isEmpty(PlayerList list) {
    return list.head == NULL;
}

int lengthList(PlayerList list) {
    int count = 0;
    Player *current = list.head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void printList(PlayerList list) {
    Player *current = list.head;
    printf("---- GIOCATORI ----\n");
    while (current != NULL) {
        printf("%s - Posizione: %d", current->name, current->position);
        if (current->finished)
            printf(" (Arrivato, Ordine: %d)", current->arrival_order);
        else if (current->skipped_turns > 0)
            printf(" (Salta %d turni)", current->skipped_turns);
        printf("\n");
        current = current->next;
    }
    printf("-------------------\n");
}

void insertFirst(PlayerList *list, char *name, int score) {
    Player *new_player = malloc(sizeof(Player));
    if (new_player == NULL) {
        printf("Errore allocazione memoria\n");
        return;
    }
    strncpy(new_player->name, name, MAX_NAME_LEN);
    new_player->position = score;
    new_player->skipped_turns = 0;
    new_player->finished = 0;
    new_player->arrival_order = 0;
    new_player->next = list->head;
    list->head = new_player;
}

void insertAfter(Player *p, char *name, int score) {
    if (p == NULL) return;

    Player *new_player = malloc(sizeof(Player));
    if (new_player == NULL) {
        printf("Errore allocazione memoria\n");
        return;
    }
    strncpy(new_player->name, name, MAX_NAME_LEN);
    new_player->position = score;
    new_player->skipped_turns = 0;
    new_player->finished = 0;
    new_player->arrival_order = 0;

    new_player->next = p->next;
    p->next = new_player;
}

void insertSorted(PlayerList *list, char *name, int pos) {
    Player *new_player = malloc(sizeof(Player));
    if (new_player == NULL) {
        printf("Errore allocazione memoria\n");
        return;
    }

    strncpy(new_player->name, name, MAX_NAME_LEN);
    new_player->position = pos;
    new_player->skipped_turns = 0;
    new_player->finished = 0;
    new_player->arrival_order = 0;
    new_player->next = NULL;

    if (isEmpty(*list) || pos > list->head->position) {
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



Player deleteFirst(PlayerList *list) {
    Player tmp = {0};
    if (isEmpty(*list)) return tmp;

    Player *to_delete = list->head;
    tmp = *to_delete;
    list->head = list->head->next;
    free(to_delete);
    return tmp;
}

Player deleteAbs(PlayerList *list, char *name) {
    Player tmp = {0};
    if (isEmpty(*list)) return tmp;

    Player *current = list->head;
    Player *previous = NULL;

    while (current != NULL && strcmp(current->name, name) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) return tmp; // non trovato

    tmp = *current;

    if (previous == NULL) {
        list->head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current);
    return tmp;
}


Player *findPlayer(PlayerList list, char *name) {
    Player *current = list.head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}


void rewindCursor(PlayerList *list) {
    list->cursor = list->head;
}

bool hasNextPlayer(PlayerList list) {
    return list.cursor != NULL;
}

Player *nextPlayer(PlayerList *list) {
    Player *current = list->cursor;
    if (list->cursor != NULL)
        list->cursor = list->cursor->next;
    return current;
}


void sortList(PlayerList *list) {
    if (list->head == NULL || list->head->next == NULL)
        return;

    PlayerList sorted = createPlayerList();

    Player *curr = list->head;
    while (curr != NULL) {
        insertSorted(&sorted, curr->name, curr->position);
        curr = curr->next;
    }

    while (!isEmpty(*list))
        deleteFirst(list);

    list->head = sorted.head;
}
