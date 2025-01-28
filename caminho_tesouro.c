#include "animacoes.h" 
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "pio_matrix.pio.h"

void desenho_pio(double *dados, PIO pio, uint sm, double vr, double vg, double vb);

void caminho_tesouro(PIO pio, uint sm) {
    // Define padrões das setas
    double arrow_right[25] = {0};
    arrow_right[2] = arrow_right[8] = arrow_right[10] = arrow_right[18] = arrow_right[22] = 1.0;
    arrow_right[11] = arrow_right[12] = arrow_right[13] = arrow_right[14] = 1.0;

    double arrow_down[25] = {0};
    arrow_down[2] = arrow_down[7] = arrow_down[12] = arrow_down[17] = arrow_down[22] = 1.0; 
    arrow_down[10] = arrow_down[11] = arrow_down[13] = arrow_down[14] = 1.0;

    double arrow_left[25] = {0};
    arrow_left[2] = arrow_left[6] = arrow_left[14] = arrow_left[16] = arrow_left[22] = 1.0;
    arrow_left[10] = arrow_left[11] = arrow_left[12] = arrow_left[13] = 1.0;

    double arrow_up[25] = {0};
    arrow_up[2] = arrow_up[7] = arrow_up[12] = arrow_up[17] = arrow_up[22] = 1.0;
    arrow_up[14] = arrow_up[8] = arrow_up[6] = arrow_up[10] = arrow_up[7] = 1.0;

    double x_shape[25] = {0};
    x_shape[0] = x_shape[4] = x_shape[6] = x_shape[8] = x_shape[12] = 
    x_shape[16] = x_shape[18] = x_shape[20] = x_shape[24] = 1.0;

    double *arrows[4] = {arrow_right, arrow_down, arrow_left, arrow_up};

    // Ciclo de animação 
    for (int frame = 0; frame < 4; frame++) {
        desenho_pio(arrows[frame], pio, sm, 1, 0, 0); // Vermelho para setas
        sleep_ms(1000);
    }

    // X final em azul
    desenho_pio(x_shape, pio, sm, 0, 0, 1);
    sleep_ms(2000);
}