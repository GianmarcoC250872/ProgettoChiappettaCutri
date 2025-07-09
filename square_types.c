#include <stdio.h>
#include "square_types.h"

const char* nomeTipoCasella(TipoCasella tipo) {
    switch (tipo) {
        case START: return "Start";
        case END: return "End";
        case NORMALE: return "Normale";
        case STOP: return "Stop";
        case TORNA_INDIETRO: return "Torna indietro";
        case AVANZA: return "Avanza";
        case POZZO: return "Pozzo";
        case BACK_TO_START: return "Torna all'inizio";
        default: return "Sconosciuto";
    }
}
