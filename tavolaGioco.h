#ifndef TAVOLAGIOCO_H_INCLUDED
#define TAVOLAGIOCO_H_INCLUDED

#include "config.h"
#include "square_types.h"

typedef enum {FISSA, ARCADE}
ModalitaGioco;

typedef struct {
    int numero;
    TipoCasella tipo;
    int effetto;
}Casella;

void inizializzaTavola(ModalitaGioco modalita);
void stampaTavolaEstesa(int posizioni[], int num_giocatori);
Casella* getCasella(int numero);

#endif // TAVOLAGIOCO_H_INCLUDED
