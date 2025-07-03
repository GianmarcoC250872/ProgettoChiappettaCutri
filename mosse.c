#include <stdio.h>
#include <stdlib.h>
#include "mosse.h"

ListaMosse* creaListaMosse() {
    ListaMosse *l = (ListaMosse *)malloc(sizeof(ListaMosse));
    if (l != NULL) {
        l->head = NULL;
        l->curN = NULL;
    }
    return l;
}

Mossa creaMossa(int turno, int id_giocatore, int dado1, int dado2, int pos_iniziale, int pos_finale, TipoCasella tipo) {
    Mossa m = (Mossa)malloc(sizeof(MossaNode));
    if (m == NULL) return NULL;

    m->turno = turno;
    m->id_giocatore = id_giocatore;
    m->dado1 = dado1;
    m->dado2 = dado2;
    m->pos_iniziale = pos_iniziale;
    m->pos_finale = pos_finale;
    m->tipo_casella = tipo;
    m->next = NULL;

    return m;
}

void inserisciMossaInTesta(ListaMosse *l, Mossa m) {
    if (l == NULL || m == NULL) return;
    m->next = l->head;
    l->head = m;
}

void stampaListaMosse(ListaMosse *l) {
    if (l == NULL) return;
    Mossa n = l->head;
    printf("\n[Mosse effettuate]\n");
    while (n != NULL) {
        printf("Turno %d - G%d: %d + %d = %d // da %d --> %d // Casella: %d\n",
               n->turno, n->id_giocatore + 1, n->dado1, n->dado2,
               n->dado1 + n->dado2,
               n->pos_iniziale, n->pos_finale, n->tipo_casella);
        n = n->next;
    }
}

void rewindMosse(ListaMosse *l) {
    if (l) l->curN = l->head;
}

bool hasNextMossa(ListaMosse *l) {
    return (l != NULL && l->curN != NULL);
}

Mossa nextMossa(ListaMosse *l) {
    if (!hasNextMossa(l)) return NULL;
    Mossa tmp = l->curN;
    l->curN = l->curN->next;
    return tmp;
}

void freeListaMosse(ListaMosse *l) {
    if (!l) return;
    Mossa current = l->head;
    while (current != NULL) {
        Mossa temp = current;
        current = current->next;
        free(temp);
    }
    free(l);
}
