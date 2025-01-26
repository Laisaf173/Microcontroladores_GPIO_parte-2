#include "animacoes.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "pio_matrix.pio.h"

// Função de animação: Onda Verde
void animacao_onda_verde() {
    int delay_tempo = 100; // Tempo entre os frames da animação

    for (int i = 0; i < 5; i++) { // Repete a animação 5 vezes
        for (int linha = 0; linha < 5; linha++) {
            double padrao_onda[25] = {0}; // Inicializa tudo apagado

            // Define a linha atual como acesa na cor verde
            for (int coluna = 0; coluna < 5; coluna++) {
                padrao_onda[linha * 5 + coluna] = 1;
            }

            // Envia o padrão para os LEDs
            desenho_pio(padrao_onda, pio, sm, 0, 0, 1);

            sleep_ms(delay_tempo);
        }
    }
}