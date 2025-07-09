#ifndef TAVOLAGIOCO_H_INCLUDED
#define TAVOLAGIOCO_H_INCLUDED

#include "config.h"
#include "square_types.h"
#include "players.h"

typedef enum {FISSA, ARCADE}
ModalitaGioco;

typedef struct{
    int numero;
    TipoCasella tipo;
    int effetto;
}Casella;


void inizializzaTavola(ModalitaGioco modalita);
Casella* getCasella(int numero);
void stampaTavola(PlayerList *lista, int num_giocatori);
void caricaCaselleDaFile(const char* filename);
void assegnaTipiCaselle(ModalitaGioco modalita);
void mescolaSpeciali(void);

#endif // TAVOLAGIOCO_H_INCLUDED
