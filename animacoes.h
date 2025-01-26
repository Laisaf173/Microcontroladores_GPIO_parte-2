
#define ANIMACOES_H
#ifndef ONDA_ALTERNADA_H
#define ONDA_ALTERNADA_H
#include "hardware/pio.h"
#include <stdint.h>
#include "pico/stdlib.h"


extern PIO pio;
extern uint32_t sm;

void animacao_cobrinha();
void desenha_frame();
void onda_alternada();

// Funçao de outras animacoes devem ser adicionadas aqui

#endif
