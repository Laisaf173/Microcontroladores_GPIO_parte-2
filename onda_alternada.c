
#include "pio_matrix.pio.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"

// Matriz de cores alternadas
float cores[5][3] = {
    {1.0, 0.0, 0.0}, // Vermelho
    {0.0, 1.0, 0.0}, // Verde
    {0.0, 0.0, 1.0}, // Azul
    {1.0, 1.0, 0.0}, // Amarelo
    {1.0, 0.0, 0.0}  // Vermelho
};

void desenho_pio(double *dados, PIO pio, uint sm, double vr, double vg, double vb);

// Padrão de onda alternada
void onda_alternada(PIO pio, uint sm) {
    int direcao = 1;  // 1 = esquerda para direita, -1 = direita para esquerda

    for (int anim = 0; anim < 10; anim++) {  // Executa 10 ciclos de animação
        float *cor_atual = cores[anim % 5];  // Alterna entre as cores definidas

        for (int passo = 0; passo < 5; passo++) {
            double padrao[25] = {0};  // Inicializa a matriz de LEDs

            // Gera a onda se movendo
            for (int linha = 0; linha < 5; linha++) {
                int coluna = (passo * direcao + linha) % 5;  // Calcula a posição da onda
                if (coluna < 0) coluna += 5;  // Ajusta para evitar valores negativos
                padrao[linha * 5 + coluna] = 1.0;  // Ativa o LED na posição correspondente
            }

            // Exibe o padrão nos LEDs
            desenho_pio(padrao, pio, sm, cor_atual[0], cor_atual[1], cor_atual[2]);
            sleep_ms(250);  // Ajusta a velocidade da animação
        }

        direcao *= -1;  // Alterna a direção da onda
    }
}
