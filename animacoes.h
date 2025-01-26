#ifndef ANIMACOES_H
#define ANIMACOES_H
#include "hardware/pio.h"
#include <stdint.h>

extern PIO pio;
extern uint32_t sm;

void animacao_cobrinha();
void desenha_frame();
void animacao_espiral();
// Funçao de outras animacoes devem ser adicionadas aqui

#endif
