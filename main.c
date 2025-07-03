#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "players.h"
#include "mosse.h"
#include "square_types.h"
#include "config.h"

int main() {
    srand(time(NULL));

    PlayerList *giocatori = creaListaGiocatori();
    ListaMosse *storico = creaListaMosse();

    inserisciGiocatoreOrdinato(giocatori, "Gimmy", 1, 0);
    inserisciGiocatoreOrdinato(giocatori, "Aurora", 1, 1);
    inserisciGiocatoreOrdinato(giocatori, "Greta", 1, 2);

    printf("Lista iniziale dei giocatori:\n");
    stampaListaGiocatori(giocatori);

    // simula 3 turni per ogni giocatore
    int turno = 1;
    for (int t = 0; t < 3; t++) {
        rewindGiocatori(giocatori);
        while (hasNextGiocatore(giocatori)) {
            Player *g = nextGiocatore(giocatori);
            int dado1 = rand() % 6 + 1;
            int dado2 = rand() % 6 + 1;
            int somma = dado1 + dado2;
            int pos_iniziale = g->position;
            g->position += somma;

            // check che non si superi la fine
            if (g->position >= MAX_SQUARES)
                g->position = MAX_SQUARES - 1;

            TipoCasella tipo = NORMALE;

            // crea e salva mossa
            Mossa nuova = creaMossa(turno, g->id, dado1, dado2, pos_iniziale, g->position, tipo);
            inserisciMossaInTesta(storico, nuova);

            printf("%s ha tirato %d + %d = %d --> nuova posizione: %d\n",
                   g->name, dado1, dado2, somma, g->position);

            turno++;
        }
    }

    // cronologia delle mosse
    printf("\nStorico delle mosse:\n");
    stampaListaMosse(storico);

    liberaListaGiocatori(giocatori);
    freeListaMosse(storico);

    return 0;
}
