#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "config.h"
#include "tavolaGioco.h"

Casella tavola[DIM_TAVOLA][DIM_TAVOLA];

typedef struct {
    int posizione;
    TipoCasella tipo;
}CasellaSpeciale;

CasellaSpeciale speciali[MAX_SPECIALI];
int num_speciali = 0;

void caricaCaselleDaFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Errore apertura caselle.txt");
        exit(1);
    }

    num_speciali = 0;
    int posizione;
    char tipo[32];
    while (fscanf(file, "%d %s", &posizione, tipo) == 2 && num_speciali < MAX_SPECIALI){
        speciali[num_speciali].posizione = posizione;

        if (strcmp(tipo, "AVANZA") == 0)
            speciali[num_speciali].tipo = AVANZA;
        else if (strcmp(tipo, "TORNA_INDIETRO") == 0)
            speciali[num_speciali].tipo = TORNA_INDIETRO;
        else if (strcmp(tipo, "STOP") == 0)
            speciali[num_speciali].tipo = STOP;
        else if (strcmp(tipo, "POZZO") == 0)
            speciali[num_speciali].tipo = POZZO;
        else if (strcmp(tipo, "BACK_TO_START") == 0)
            speciali[num_speciali].tipo = BACK_TO_START;
        else
            speciali[num_speciali].tipo = NORMALE;

        num_speciali++;
    }
    fclose(file);
}

void mescolaSpeciali() {
    for (int i = num_speciali - 1; i > 0; i--){
        int j = rand() % (i + 1);
        CasellaSpeciale temp = speciali[i];
        speciali[i] = speciali[j];
        speciali[j] = temp;
    }
}

void assegnaTipiCaselle(ModalitaGioco modalita) {
    caricaCaselleDaFile("caselle.txt");

    if (modalita == ARCADE)
        mescolaSpeciali();

    for (int k = 0; k < num_speciali; k++){
        int posizione = speciali[k].posizione;
        Casella* c = getCasella(posizione);
        if (c){
            c->tipo = speciali[k].tipo;

            if (c->tipo == AVANZA || c->tipo == TORNA_INDIETRO){
                c->effetto = (rand() % 5) + 1;
            } else {
                c->effetto = 0;
            }
        }
    }
}

void inizializzaTavola(ModalitaGioco modalita){
    int num = 1;
    int top = 0, bottom = DIM_TAVOLA - 1;
    int left = 0, right = DIM_TAVOLA - 1;

    while (num <= MAX_CASELLE) {
        for (int i = left; i <= right && num <= MAX_CASELLE; i++)
            tavola[top][i] = (Casella){num++, NORMALE, 0};
        top++;

        for (int i = top; i <= bottom && num <= MAX_CASELLE; i++)
            tavola[i][right] = (Casella){num++, NORMALE, 0};
        right--;

        for (int i = right; i >= left && num <= MAX_CASELLE; i--)
            tavola[bottom][i] = (Casella){num++, NORMALE, 0};
        bottom--;

        for (int i = bottom; i >= top && num <= MAX_CASELLE; i--)
            tavola[i][left] = (Casella){num++, NORMALE, 0};
        left++;
    }

    srand(time(NULL));
    assegnaTipiCaselle(modalita);
}

Casella* getCasella(int numero){
    if (numero < 1 || numero > MAX_CASELLE)
        return NULL;

    for (int i = 0; i < DIM_TAVOLA; i++){
        for (int j = 0; j < DIM_TAVOLA; j++){
            if (tavola[i][j].numero == numero)
                return &tavola[i][j];
        }
    }

    return NULL;
}

void stampaTavolaEstesa(int posizioni[], int num_giocatori) {
    for (int i = 0; i < DIM_TAVOLA; i++){

        //Riga superiore
        for (int j = 0; j < DIM_TAVOLA; j++){
            printf("[%02d] ", tavola[i][j].numero);
        }
        printf("\n");

        //Riga inferiore
        for (int j = 0; j < DIM_TAVOLA; j++){
            int numero_casella = tavola[i][j].numero;
            char cella[6] = "-";
            char buffer[6] = "";
            int count = 0;

            //check giocatori nella casella
            for (int p = 0; p < num_giocatori; p++){
                if (posizioni[p] == numero_casella){
                    if (count == 0)
                        snprintf(buffer, sizeof(buffer), "G%d", p + 1);
                    else
                        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "/G%d", p + 1);
                    count++;
                }
            }

            if (count > 0) {
                snprintf(cella, sizeof(cella), "%s", buffer);
            } else{
                Casella *c = getCasella(numero_casella);
                if (c->tipo == AVANZA) strncpy(cella, "AV", sizeof(cella));
                else if (c->tipo == TORNA_INDIETRO) strncpy(cella, "TI", sizeof(cella));
                else if (c->tipo == POZZO) strncpy(cella, "PZ", sizeof(cella));
                else if (c->tipo == BACK_TO_START) strncpy(cella, "IN", sizeof(cella));
                else if (c->tipo == STOP) strncpy(cella, "ST", sizeof(cella));
                else strncpy(cella, "-", sizeof(cella));
            }

            printf(" %-4s", cella);
        }
        printf("\n");
    }
}
