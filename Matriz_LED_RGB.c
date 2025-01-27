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

PIO pio;
uint32_t sm;
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

// Função que simula um coração pulsante 
void coracao_pulsante(PIO pio, uint sm);

// Função para desenhar uma seta
void animacao_seta(PIO pio, uint sm);
// Função para criar padrão de círculo
void criar_circulo(double *padrao, int raio);   

// Função para desenhar o padrão de círculo no PIO  
void desenhar_circulo(PIO pio, uint sm);    

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
            coracao_pulsante(pio, sm);
            break;
        case '1': // jogo da cobrinha
            animacao_cobrinha();
            break; 
        case '2': // círculo expandindo
            desenhar_circulo(pio, sm);
            break;
        case '3': // Animação 4
            animacao_espiral(pio, sm);
            break;
        case '4': // Animação 5 
            break;
        case '5': //  Animação de seta pulsante com mudança automática de cores
            animacao_seta(pio, sm);
            break;
        case '6': // Animação 7
            break;
         case '7': // Aciona a animação de onda alternada
            onda_alternada(pio, sm);
            break;
        case 'A': // Desligar todos os LEDs
            acender_leds(0, 0, 0);
            break;
        case 'B': // Acionamento de todos os LEDs em azul - intensidade 100%
            acender_leds(0, 0, 1);
            break;
        case 'C': // Acionamento de todos os LEDs em vermelho - intensidade 80%
            acender_leds(0.8, 0, 0);
            break;
        case 'D': // Acionamento de todos os LEDs em verde - intensidade 50%
            acender_leds(0, 0.5, 0);
            break;
        case '#': // Acionamento de todos os LEDs em branco - intensidade 20%
            acender_leds(0.2, 0.2, 0.2);
            break;
        case '*': // Reboot do sistema
            reset_usb_boot(0, 0);
            break;
        default:
            printf("Tecla não mapeada\n");
            break;
    }
}

// Padrão do coração ajustado para 5x5
double coracao_pequeno[25] = {
    0, 0, 0, 0, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0
};

double coracao_medio[25] = {
    0, 1, 0, 1, 0,
    1, 1, 1, 1, 1,
    0, 1, 1, 1, 0,
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0
};

double coracao_grande[25] = {
    1, 1, 0, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0
};
// Função animacao_seta adicionada
double seta[25] = {
    0, 0, 1, 0, 0,
    0, 1, 1, 1, 0,
    1, 1, 1, 1, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0
};

double seta1[25] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    1, 1, 1, 1, 1,
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0
};

void animacao_seta(PIO pio, uint sm) {
    float cores[5][3] = {
        {1.0, 0.0, 1.0}, 
        {1.0, 0.0, 0.0}, 
        {0.0, 1.0, 0.0},
        {1.0, 1.0, 0.0}, 
        {0.0, 0.0, 1.0}  
    };
    double *matriz[5] = {seta, seta1};
    for(int i = 0; i < 25; i++) {
        float *cor_atual = cores[i % 5];
        for(int j = 0; j < 2; j++) {
            desenho_pio(matriz[j], pio, sm, cor_atual[0], cor_atual[1], cor_atual[2]);
            sleep_ms(200);
        }
    }
}
void coracao_pulsante(PIO pio, uint sm) {
    // Array de cores (R, G, B)
    float cores[5][3] = {
        {1.0, 0.0, 0.0}, // Vermelho
        {0.0, 1.0, 0.0}, // Verde
        {0.0, 0.0, 1.0}, // Azul
        {1.0, 1.0, 0.0}, // Amarelo
        {1.0, 0.0, 1.0}  // Magenta
    };

    // Simula 5 batidas de coração
    for(int beat = 0; beat < 5; beat++) {
        // Seleciona a cor atual
        float *cor_atual = cores[beat % 5];

        // Pulso rápido para cima
        desenho_pio(coracao_pequeno, pio, sm, cor_atual[0], cor_atual[1], cor_atual[2]);
        sleep_ms(100);
        desenho_pio(coracao_medio, pio, sm, cor_atual[0], cor_atual[1], cor_atual[2]);
        sleep_ms(100);
        desenho_pio(coracao_grande, pio, sm, cor_atual[0], cor_atual[1], cor_atual[2]);
        sleep_ms(100);
        
        // Pulso rápido para baixo
        desenho_pio(coracao_medio, pio, sm, cor_atual[0], cor_atual[1], cor_atual[2]);
        sleep_ms(100);
        desenho_pio(coracao_pequeno, pio, sm, cor_atual[0], cor_atual[1], cor_atual[2]);
        sleep_ms(100);
        
        // Pausa entre batidas
        desenho_pio(coracao_pequeno, pio, sm, cor_atual[0] * 0.5, cor_atual[1] * 0.5, cor_atual[2] * 0.5); // Mais fraco
        sleep_ms(300);
    }
}

// Função para criar padrão de círculo
void criar_circulo(double *padrao, int raio) {
    // Loop para percorrer a matriz 5x5
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            // Calcula a distância do centro (2,2) até a posição atual
            int distancia = abs(i - 2) + abs(j - 2);
            // Se a distância for menor ou igual ao raio, define o valor como 1.0 (círculo)
            padrao[i * 5 + j] = (distancia <= raio) ? 1.0 : 0.0;
        }
    }
}

void desenhar_circulo(PIO pio, uint sm) {
    // Define a matriz 5x5 para armazenar o padrão do círculo
    double padrao_circulo[25] = {0};
    // Define as cores para a animação
    float cores[5][3] = {
        {1.0, 0.0, 0.0}, // Vermelho
        {0.0, 1.0, 0.0}, // Verde
        {0.0, 0.0, 1.0}, // Azul
        {1.0, 1.0, 0.0}, // Amarelo
        {1.0, 0.0, 1.0}  // Magenta
    };

    // Loop para realizar a animação 5 vezes
    for (int anim = 0; anim < 5; anim++) {
        // Seleciona a cor atual para a animação
        float *cor_atual = cores[anim % 5];

        // Loop para aumentar o raio do círculo
        for (int raio = 0; raio <= 2; raio++) {
            // Cria o padrão do círculo com o raio atual
            criar_circulo(padrao_circulo, raio);
            // Desenha o padrão do círculo no PIO com a cor atual
            desenho_pio(padrao_circulo, pio, sm, cor_atual[0], cor_atual[1], cor_atual[2]);
            // Pausa por 100ms
            sleep_ms(100);
        }

        // Loop para diminuir o raio do círculo
        for (int raio = 2; raio >= 0; raio--) {
            // Cria o padrão do círculo com o raio atual
            criar_circulo(padrao_circulo, raio);
            // Desenha o padrão do círculo no PIO com a cor atual
            desenho_pio(padrao_circulo, pio, sm, cor_atual[0], cor_atual[1], cor_atual[2]);
            // Pausa por 100ms
            sleep_ms(100);
        }
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
