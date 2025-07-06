#ifndef PLAYERS_H_INCLUDED
#define PLAYERS_H_INCLUDED

#include <stdbool.h>
#include "config.h"

typedef enum{LIBERO, BLOCCATO, NEL_POZZO, ARRIVATO}
StatoGiocatore;

typedef struct Player {
    char name[MAX_NAME_LEN];
    int id;
    int position;
    StatoGiocatore stato;
    int turniNelPozzo;
    struct Player *next;
}Player;

typedef struct {
    Player *head;
    Player *cursor;
}PlayerList;

PlayerList* creaListaGiocatori();
void inserisciGiocatoreInTesta(PlayerList *list, char *name, int pos, int id);
void inserisciGiocatoreOrdinato(PlayerList *list, char *name, int pos, int id);
void stampaListaGiocatori(PlayerList *list);
int lunghezzaListaGiocatori(PlayerList *list);
bool listaGiocatoriVuota(PlayerList *list);
Player *cercaGiocatore(PlayerList *list, char *name);
void rewindGiocatori(PlayerList *list);
bool hasNextGiocatore(PlayerList *list);
Player *nextGiocatore(PlayerList *list);
void liberaListaGiocatori(PlayerList *list);

#endif // PLAYERS_H_INCLUDED
