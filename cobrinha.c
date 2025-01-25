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
    double leds[NUM_PIXELS] = {0};  // Começa com todos os LEDs apagados

    // Posições iniciais da cobrinha (corpo e cabeça) com 3 LEDs
    int corpo[3] = {0, 1, 2};  // Corpo da cobrinha com 3 LEDs

    // Loop para controlar o movimento da cobrinha
    for (int linha = 0; linha < 5; linha++) {
        // Preenche a linha com padrão zigzag
        if (linha % 2 == 0) { // Linha par (esquerda para direita)
            for (int coluna = 0; coluna < 5; coluna++) {
                // Apaga o último LED da cobrinha (a cauda)
                leds[corpo[0]] = 0;

                // Move o corpo da cobrinha
                for (int i = 1; i < 3; i++) {
                    corpo[i - 1] = corpo[i]; // Move o corpo para a frente
                }
                corpo[2] = linha * 5 + coluna;  // A cabeça se move para o próximo LED

                // Acende os LEDs para representar a cobrinha
                for (int i = 0; i < 3; i++) {
                    leds[corpo[i]] = 1;  // Acende a cabeça e o corpo
                }

                // Desenha o frame
                desenha_frame(leds);
                sleep_ms(200);
            }
        } else { // Linha ímpar (direita para esquerda)
            for (int coluna = 4; coluna >= 0; coluna--) {
                // Apaga o último LED da cobrinha (a cauda)
                leds[corpo[0]] = 0;

                // Move o corpo da cobrinha
                for (int i = 1; i < 3; i++) {
                    corpo[i - 1] = corpo[i]; // Move o corpo para a frente
                }
                corpo[2] = linha * 5 + coluna;  // A cabeça se move para o próximo LED

                // Acende os LEDs para representar a cobrinha
                for (int i = 0; i < 3; i++) {
                    leds[corpo[i]] = 1;  // Acende a cabeça e o corpo
                }

                // Desenha o frame
                desenha_frame(leds);
                sleep_ms(200);
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
