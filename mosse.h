#ifndef MOSSE_H_INCLUDED
#define MOSSE_H_INCLUDED

#include <stdbool.h>
#include "square_types.h"

typedef struct MossaNode {
    int turno;
    int id_giocatore;
    int dado1;
    int dado2;
    int pos_iniziale;
    int pos_finale;
    TipoCasella tipo_casella;

    struct MossaNode *next;
} MossaNode;

typedef MossaNode* Mossa;

typedef struct {
    Mossa head;
    Mossa curN;
} ListaMosse;

ListaMosse* creaListaMosse();
Mossa creaMossa(int turno, int id_giocatore, int dado1, int dado2, int pos_iniziale, int pos_finale, TipoCasella tipo);
void inserisciMossaInTesta(ListaMosse *lista, Mossa m);
void stampaListaMosse(ListaMosse *lista);
void rewindMosse(ListaMosse *lista);
bool hasNextMossa(ListaMosse *lista);
Mossa nextMossa(ListaMosse *lista);
void freeListaMosse(ListaMosse *lista);

#endif // MOSSE_H_INCLUDED
