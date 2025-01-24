#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "animacoes.h"
#include "hardware/clocks.h"

//arquivo .pio
#include "pio_matrix.pio.h"

PIO pio = pio0;  
uint32_t sm = 0;     
static uint offset;

//número de LEDs
#define NUM_PIXELS 25

//pino de saída
#define OUT_PIN 7


#define ROWS 4
#define COLS 4

const char KEYS[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

int matriz_mapeamento_LEDS[25] = {
    24, 23, 22, 21, 20,  
    15, 16, 17, 18, 19, 
    14, 13, 12, 11, 10,  
     5,  6,  7,  8,  9,  
     4,  3,  2,  1,  0   
};

// Definição dos pinos do teclado
uint GPIO_COLS[4] = {28, 27, 26, 22}; 
uint GPIO_ROWS[4] = {0, 1, 2, 3};


//Buzzer
const uint GPIO_BUZZER = 21;

//imprimir valor binário
void imprimir_binario(int num); 

//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g);

//Configurar os pinos
void init_gpio(void);

// Função de controle do buzzer
void control_buzzer(char key);

// Função para ler a tecla pressionada
char read_key(void);

// Funções de controle dos LEDs
void desenho_pio(double *dados, PIO pio, uint sm, double vr, double vg, double vb);

//funçao para acender os leds
void acender_leds(double r, double g, double b) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        uint32_t cor = matrix_rgb(b, r, g);
        pio_sm_put_blocking(pio, sm, cor);
    }
}

// Funções de controle dos LEDs
void controle_animacoes(char key) {
    switch (key) {
        case '0': // Animação 1
            break;
        case '1': // jogo da cobrinha
            animacao_cobrinha();
            break; 
        case '2':  // Animação 3
            break;
        case '3': // Animação 4
            break;
        case '4': // Animação 5 
            break;
        case '5': // Animação 6
            break;
        case '6': // Animação 7
            break;
        case '7': // Animação 8
            break;
        case 'A': // Desligar todos os LEDs
            break;
        case 'B': // Acionamento de todos os LEDs em azul - intensidade 100%
            acender_leds(0, 0, 1);
            break;
        case 'C': // Acionamento de todos os LEDs em vermelho - intensidade 80%
            break;
        case 'D': // Acionamento de todos os LEDs em verde - intensidade 50%
            break;
        case '#': // Acionamento de todos os LEDs em branco - intensidade 20%
            break;
        case '*': // Reboot do sistema
            reset_usb_boot(0, 0);
            break;
        default:
            printf("Tecla não mapeada\n");
            break;
    }
}


int main() {
    pio = pio0; 
    offset = pio_add_program(pio, &pio_matrix_program);
    sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    stdio_init_all();
    init_gpio();
    
    while (true) {
        char key = read_key();
        if (key != '\0') {
            controle_animacoes(key);
        }
        sleep_ms(100);
    }

    return 0;
}

//imprimir valor binário
void imprimir_binario(int num) {
    int i;
    for (i = 31; i >= 0; i--) {
    (num & (1 << i)) ? printf("1") : printf("0");
    }
}

//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g){
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

//Configurar os pinos
void init_gpio(void) {
    for (int i = 0; i < ROWS; i++) {
        gpio_init(GPIO_ROWS[i]);
        gpio_set_dir(GPIO_ROWS[i], GPIO_OUT);
        gpio_put(GPIO_ROWS[i], 0);
    }

    for (int i = 0; i < COLS; i++) {
        gpio_init(GPIO_COLS[i]);
        gpio_set_dir(GPIO_COLS[i], GPIO_IN);
        gpio_pull_down(GPIO_COLS[i]);
    }

    gpio_init(GPIO_BUZZER);
    gpio_set_dir(GPIO_BUZZER, GPIO_OUT);
    gpio_put(GPIO_BUZZER, 0);
}

// Função de controle do buzzer
void control_buzzer(char key) {
    if (key == 'A') {
        gpio_put(GPIO_BUZZER, 1); // Ativa o buzzer
        sleep_ms(500);  // Mantém o buzzer ligado por 500ms
        gpio_put(GPIO_BUZZER, 0); // Desativa o buzzer
    }
}

// Função para ler a tecla pressionada
char read_key(void) {
    for (int row = 0; row < ROWS; row++) {
        gpio_put(GPIO_ROWS[row], 1);
        for (int col = 0; col < COLS; col++) {
            if (gpio_get(GPIO_COLS[col])) {
                gpio_put(GPIO_ROWS[row], 0);
                sleep_ms(50); // Adiciona debouncing
                return KEYS[row][col];
            }
        }
        gpio_put(GPIO_ROWS[row], 0);
    }
    return '\0'; // Retorna nulo se nenhuma tecla for pressionada
}


void desenho_pio(double *dados, PIO pio, uint sm, double vr, double vg, double vb) {
    for (int px = 0; px < NUM_PIXELS; px++) {
        uint16_t pos = matriz_mapeamento_LEDS[px]; // Obtém posição real no strip
        float intensidade = dados[pos];
        
        uint32_t cor = matrix_rgb(
            (uint8_t)(intensidade * vr),
            (uint8_t)(intensidade * vg), 
            (uint8_t)(intensidade * vb)
        );
        
        pio_sm_put_blocking(pio, sm, cor);
    }
}