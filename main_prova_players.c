

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "players.h"

int main() {
    PlayerList list = createPlayerList();

    printf("Lista vuota? %s\n", isEmpty(list) ? "YES" : "NO");

    printf("\nInserisco tre giocatori con posizioni diversi:\n");
    insertSorted(&list, "Gimmy", 5);
    insertSorted(&list, "Aurora", 8);
    insertSorted(&list, "Greta", 3);

    printList(list);

    printf("\nNumero di giocatori: %d\n", lengthList(list));

    printf("\nCerco 'Gimmy':\n");
    Player *found = findPlayer(list, "Gimmy");
    if (found) {
        printf("Trovato: %s alla posizione %d\n", found->name, found->position);
    } else {
        printf("Giocatore non trovato\n");
    }

    printf("\nUso il cursore per scorrere i giocatori:\n");
    rewindCursor(&list);
    while (hasNextPlayer(list)) {
        Player *p = nextPlayer(&list);
        printf("%s alla posizione %d\n", p->name, p->position);
    }

    printf("\nElimino 'Gimmy' dalla lista:\n");
    deleteAbs(&list, "Gimmy");
    printList(list);

    printf("\nElimino il primo elemento:\n");
    deleteFirst(&list);
    printList(list);

    printf("\nInserisco 'Marco' e 'Mario' con ordinamento automatico:\n");
    insertSorted(&list, "Marco", 10);
    insertSorted(&list, "Mario", 2);
    printList(list);

    printf("\nRiordino la lista manualmente:\n");
    sortList(&list);
    printList(list);

    return 0;
}
