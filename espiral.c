#include "animacoes.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "pio_matrix.pio.h"

#define NUM_PIXELS 25

extern int matriz_mapeamento_LEDS[25];

// Função de animação em espiral
void animacao_espiral(PIO pio, uint sm) {
    double leds[NUM_PIXELS] = {0};  // Inicializa todos os LEDs apagados

    // Definição do padrão de espiral (ordem em que os LEDs devem acender)
    int espiral[NUM_PIXELS] = {
        0, 1, 2, 3, 4,
        9, 14, 19, 24,
        23, 22, 21, 20,
        15, 10, 5,
        6, 7, 8,
        13, 18,
        17, 16,
        11, 12
    };

    // Loop para animar o padrão de espiral
    for (int i = 0; i < NUM_PIXELS; i++) {
        leds[espiral[i]] = 1;  // Acende o LED atual
        desenha_frame(leds);   // Atualiza o frame na matriz
        sleep_ms(200);         // Pausa para visualizar o movimento
    }

    // Apaga os LEDs em ordem inversa (efeito de "desligar em espiral")
    for (int i = NUM_PIXELS - 1; i >= 0; i--) {
        leds[espiral[i]] = 0;  // Apaga o LED atual
        desenha_frame(leds);   // Atualiza o frame na matriz
        sleep_ms(200);         // Pausa para visualizar o movimento
    }
}