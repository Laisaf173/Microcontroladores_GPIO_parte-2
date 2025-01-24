#include "animacoes.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "pio_matrix.pio.h"

#define NUM_PIXELS 25
extern int matriz_mapeamento_LEDS[25];

// Função auxiliar para definir a cor azul
uint32_t azul(double intensidade) {
    return (uint32_t)(intensidade * 255) << 8; // Define a cor azul
}

// Função de animação cobrinha em zigzag
void animacao_cobrinha(PIO pio, uint sm) {
    double leds[NUM_PIXELS] = {0};

    // Loop para controlar o movimento da cobrinha
    for (int linha = 0; linha < 5; linha++) {
        // Preenche a linha com padrão zigzag
        if (linha % 2 == 0) { // Linha par (esquerda para direita)
            for (int coluna = 0; coluna < 5; coluna++) {
                leds[linha * 5 + coluna] = 1; // Acende LED
                desenha_frame(leds);
                sleep_ms(200);
                leds[linha * 5 + coluna] = 0; // Apaga LED
            }
        } else { // Linha ímpar (direita para esquerda)
            for (int coluna = 4; coluna >= 0; coluna--) {
                leds[linha * 5 + coluna] = 1; // Acende LED
                desenha_frame(leds);
                sleep_ms(200);
                leds[linha * 5 + coluna] = 0; // Apaga LED
            }
        }
    }
}

// Função para desenhar o frame na matriz de LEDs
void desenha_frame(double *leds) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        uint16_t pos = matriz_mapeamento_LEDS[i]; // Obtém posição real no strip
        double intensidade = leds[pos];
        uint32_t cor = azul(intensidade); // Define cor azul
        pio_sm_put_blocking(pio, sm, cor);
    }
}
