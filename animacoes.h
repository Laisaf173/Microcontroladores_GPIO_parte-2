
#ifndef ANIMACOES_H
#define ANIMACOES_H
#include "hardware/pio.h"
#include <stdint.h>
#include "pico/stdlib.h"


extern PIO pio;
extern uint32_t sm;

void animacao_cobrinha();
void onda_alternada();
void animacao_espiral();
void desenha_frame();

// Funçao de outras animacoes devem ser adicionadas aqui

#endif
