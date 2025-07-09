#ifndef SQUARE_TYPES_H_INCLUDED
#define SQUARE_TYPES_H_INCLUDED

#include "config.h"

//L'header definisce i tipi di caselle nel gioco e fornisce una funzione per ottenere il nome testuale del tipo di caselle

typedef enum{
    START,
    END,
    NORMALE,
    STOP,
    TORNA_INDIETRO,
    AVANZA,
    POZZO,
    BACK_TO_START
} TipoCasella;

const char* nomeTipoCasella(TipoCasella tipo);//prende in input un valore di tipo "TipoCasella" e restituisce una stringa costante (const char*)

#endif // SQUARE_TYPES_H_INCLUDED
