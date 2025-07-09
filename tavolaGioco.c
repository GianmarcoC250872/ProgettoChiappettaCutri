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
        perror("Errore apertura 'caselle.txt'");
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
    for (int i = MAX_SPECIALI - 1; i > 0; i--){
        int j = rand() % (i + 1);
        CasellaSpeciale temp = speciali[i];
        speciali[i] = speciali[j];
        speciali[j] = temp;
    }
}

void assegnaTipiCaselle(ModalitaGioco modalita) {
    caricaCaselleDaFile("caselle.txt");

    //salva le posizioni originali
    int posizioni[MAX_SPECIALI];
    TipoCasella tipi[MAX_SPECIALI];

    for (int i = 0; i < num_speciali; i++) {
        posizioni[i] = speciali[i].posizione;
        tipi[i] = speciali[i].tipo;
    }

    //mescola le caselle
    if (modalita == ARCADE) {
        for (int i = num_speciali - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            TipoCasella tmp = tipi[i];
            tipi[i] = tipi[j];
            tipi[j] = tmp;
        }
    }

    //assegna i tipi alle posizioni con eventuale effetto
    for (int i = 0; i < num_speciali; i++) {
        Casella *c = getCasella(posizioni[i]);
        if (c) {
            c->tipo = tipi[i];
            if (c->tipo == AVANZA || c->tipo == TORNA_INDIETRO) {
                c->effetto = (rand() % 5) + 1;
            } else {
                c->effetto = 0;
            }
        }
    }
}

void inizializzaTavola(ModalitaGioco modalita){
    int num = 0;
    int top = 0, bottom = DIM_TAVOLA - 1;
    int left = 0, right = DIM_TAVOLA - 1;

    while (num < MAX_CASELLE) {
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

    srand(time(NULL)); //inizializzazione del seed per generazioni casuali

    // Imposta START ed END
    Casella* start = getCasella(0);
    if (start) start->tipo = START;

    Casella* end = getCasella(MAX_CASELLE - 1);
    if (end) end->tipo = END;

    assegnaTipiCaselle(modalita);
}

Casella* getCasella(int numero){
    if (numero < 0 || numero >= MAX_CASELLE)
        return NULL;

    for (int i = 0; i < DIM_TAVOLA; i++){
        for (int j = 0; j < DIM_TAVOLA; j++){
            if (tavola[i][j].numero == numero)
                return &tavola[i][j];
        }
    }

    return NULL;
}

void stampaTavola(PlayerList *lista, int num_giocatori) {

    //11 CARATTERI MAX PER CASELLA
    //dichiarazione di un vettore contenente colori ANSI
    const char* colori[4] = {
        "\033[1;31m", //Rosso
        "\033[1;32m", //Verde
        "\033[1;34m", //Blu
        "\033[1;33m"  //Giallo
    };

    int max_casella = DIM_TAVOLA * DIM_TAVOLA - 1;

    for (int i = 0; i < DIM_TAVOLA; i++) { //scorrimento righe tavola

        //Riga 1 della casella: bordo superiore
        for (int j = 0; j < DIM_TAVOLA; j++)
            printf("+-----------");
        printf("+\n");

        //Riga 2 della casella: numero + tipo
        for (int j = 0; j < DIM_TAVOLA; j++) {
            int numero = tavola[i][j].numero;
            Casella *c = getCasella(numero);

            char tipo[6] = "--";
            if (numero == 0) strncpy(tipo, "START", 6);
            else if (numero == max_casella) strncpy(tipo, "END", 6);
            else if (c->tipo == AVANZA) strncpy(tipo, "FRWD", 6);
            else if (c->tipo == TORNA_INDIETRO) strncpy(tipo, "BACK", 6);
            else if (c->tipo == POZZO) strncpy(tipo, "POZZ", 6);
            else if (c->tipo == BACK_TO_START) strncpy(tipo, "BT_S", 6);
            else if (c->tipo == STOP) strncpy(tipo, "STOP", 6);
            else strncpy(tipo, "--", 6);

            if (numero == 0) {
                printf("|   START   ");
            } else if (numero == max_casella) {
                printf("|    END    ");
            } else {
                printf("| [%02d] %-5s", numero, tipo);
            }
        }
        printf("|\n"); //bordo dx

        // Riga 3: giocatori centrati con spazio e colore ANSI
        for (int j = 0; j < DIM_TAVOLA; j++) {
            int numero = tavola[i][j].numero;

            char buffer[128] = ""; //inizializzazione del buffer
            int len_riempimento = 0;

            Player *curr = lista->head;
            while (curr != NULL) {
                if (curr->position == numero) {
                    //definizione di un buffer per snprintf
                    char tmp[32];
                    //snprintf --> stampa controllata da un buffer su una stringa
                    snprintf(tmp, sizeof(tmp), "%sG%d\033[0m ", colori[curr->colore_id], curr->id); //colore + Gx + reset colore + spazio
                    strcat(buffer, tmp);
                    len_riempimento += 3; //tiene traccia di quanti giocatori sono nella casella (Gx + spazio = 3)
                }
                curr = curr->next; //passa al next player
            }

            // Rimuove spazio finale tramite buffer salvato in size_t (int not negative)
            size_t len = strlen(buffer);

            //check che il buffer non sia vuoto e che l'ultimo carattere sia uno spazio,
            //eventualmente lo sostituisce con il terminatore di stringa
            if (len > 0 && buffer[len - 1] == ' '){
                buffer[len - 1] = '\0';
                len_riempimento--;
            }

            //testo della riga centrato
            int len_tot = 11 - len_riempimento;
            if (len_tot < 0)
                len_tot = 0;
            int len_left = len_tot / 2;
            int len_right = len_tot - len_left;

            printf("|%*s%s%*s", len_left, "", buffer, len_right, "");
        }
        printf("|\n"); //bordo dx

        // Riga 4: bordo inferiore
        for (int j = 0; j < DIM_TAVOLA; j++)
            printf("+-----------");
        printf("+\n");
    }

    printf("\033[0m"); // Reset colore
}

