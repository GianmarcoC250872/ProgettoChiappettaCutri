#ifndef SQUARE_TYPES_H_INCLUDED
#define SQUARE_TYPES_H_INCLUDED

#include "config.h"

typedef enum{
    NORMALE,
    STOP,
    TORNA_INDIETRO,
    AVANZA,
    POZZO,
    BACK_TO_START
}TipoCasella;

const char* nomeTipoCasella(TipoCasella tipo);

#endif // SQUARE_TYPES_H_INCLUDED
