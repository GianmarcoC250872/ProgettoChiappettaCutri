#ifndef PLAYERS_H_INCLUDED
#define PLAYERS_H_INCLUDED

#define MAX_NAME_LEN 20
#define MAX_PLAYERS 4
#define START_POS 0
#include <stdbool.h>

typedef struct Player {
    char name[MAX_NAME_LEN];
    int position;
    int skipped_turns;
    int finished;
    int arrival_order;
    struct Player *next;
} Player;

typedef struct {
    Player *head;
    Player *cursor;
} PlayerList;


PlayerList createPlayerList();
bool isEmpty(PlayerList list);
int lengthList(PlayerList list);
void printList(PlayerList list);


void insertFirst(PlayerList *list, char *name, int score);
void insertAfter(Player *p, char *name, int score);
void insertSorted(PlayerList *list, char *name, int score);


Player deleteFirst(PlayerList *list);
Player deleteAbs(PlayerList *list, char *name);


Player *findPlayer(PlayerList list, char *name);


void rewindCursor(PlayerList *list);
bool hasNextPlayer(PlayerList list);
Player *nextPlayer(PlayerList *list);

void sortList(PlayerList *list);

#endif // PLAYERS_H_INCLUDED
