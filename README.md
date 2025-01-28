# Microcontroladores_GPIO_parte-2

## 🔍Visão Geral:
Este projeto implementa um sistema baseado no microcontrolador Raspberry Pi Pico, utilizando:
- Uma matriz de LEDs (5x5)
- Um teclado matricial (4x4)
- Um buzzer para feedback sonoro.

O código permite o **controle** e **animação** dos LEDs com base nas teclas pressionadas no teclado matricial. Cada tecla está associada a uma animação ou comportamento específico. Vale destacar que a simulação ocorre por meio da extensão Wokwi. 

## 🗺️ Mapeamento das teclas
| Tecla | Funcionalidade |
| ------ | ------ |
| '0' a '7' | Diferentes animações |
| 'A' | Desliga todos os LEDs|
| 'B' | LEDs azuis (100%) |
| 'C'  | LEDs vermelhos (80%)|
| 'D'| LEDs verdes (50%) |
| '#'| LEDs brancos (20%)|
| '*' |Reboot do sistema|

## 🪄 Animações implementadas  

| Animação | Tecla |
| ------ | ------ |
| 💖 Coração pulsante | 0 |
| 🐍 Jogo da cobrinha | 1|
| 🔘 Círculo expansivo | 2 |
| 🌀 Espiral  | 3|
| 💰 Caminho do tesouro| 4 |
| ↗️ Seta alternada| 5|
| 🔲 Quadrado crescente | 6|
| 🌊 Onda alternada |7 |

## 🙋‍♀️ Autores 🙋 

- Cauã Brizolara ---> Animação "🔘 Círculo expansivo" e acionamento Tecla C
- Helga Maria --->  Animação " ↗️ Seta alternada" e acionamento Tecla #
- Juliano Neves --->  Animação "🌊 Onda alternada"
- Laisa França (Líder da semana) --->  Animação "💖 Coração pulsante" e acionamento tecla A
- Luis César --->  Animação "🔲 Quadrado crescente"
- Marcos Silveira --->  Animação "💰 Caminho do tesouro"
- Rian Barroso --->  Animação "🐍 Jogo da cobrinha" e acionamento tecla B, D e *
- Vitor Freire --->  Animação "🌀 Espiral"

Link do vídeo com o resultado final e código do projeto: https://drive.google.com/file/d/1egu9srgPMBwic0xA0CLCz8Q9SdJyYDrF/view?usp=sharing
