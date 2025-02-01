#include <stdio.h>            // Inclusão da biblioteca padrão de entrada/saída, necessária para funções como printf() para exibição de mensagens no terminal.
#include "pico/stdlib.h"      // Inclusão da biblioteca padrão da Raspberry Pi Pico, que oferece funções de controle como GPIO, temporizadores, e outros recursos.
#include "hardware/gpio.h"    // Biblioteca para manipulação de pinos GPIO no Raspberry Pi Pico, permitindo configurações e controle de entradas e saídas digitais.
#include "hardware/timer.h"   // Biblioteca para controle e utilização de temporizadores (timers) do Raspberry Pi Pico, úteis para delays e outras funções temporizadas.
#include "hardware/pio.h"     // Biblioteca para trabalhar com o PIO (Programmable Input/Output), uma funcionalidade avançada do Raspberry Pi Pico que permite implementar protocolos personalizados de comunicação e controle de hardware.
#include "ws2812.h"           // Biblioteca PIO para controle dos WS2812

// Definição dos pinos
#define LED_G 11  // Pino para o LED Verde
#define LED_B 12  // Pino para o LED Azul
#define LED_R 13  // Pino para o LED Vermelho
#define BTN_A 5
#define BTN_B 6
#define MATRIX_PIN 7  // Pino da Matriz WS2812
#define INTENSIDADE 10 // Define a intensidade dos LEDs (escala de 0 a 255)
#define MATRIX_SIZE 25 // Configuração da Matriz 5x5 (25 LEDs WS2812)
uint32_t leds[MATRIX_SIZE]; // Buffer de cores para a matriz
int numero_atual = 0;

// Controle do LED RGB
bool led_state = false;

// PIO e estado do programa
PIO pio = pio0;
int sm;

// Função de callback do temporizador (pisca o LED vermelho)
bool blink_led_callback(struct repeating_timer *t) {
    led_state = !led_state;
    gpio_put(LED_R, led_state);
    return true;  // Mantém o temporizador ativo
}

// Mapeamento dos números para exibição na matriz 5x5
const uint32_t numeros[10][25] = {
    // Representações gráficas dos números (0-9) em 5x5

 // 0 
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    // 1 
    {1, 1, 1, 1, 1,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 1, 1, 0, 0,
     0, 0, 1, 0, 0},

    // 2 
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     0, 1, 1, 1, 1},

    // 3 
    {1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 0, 0,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    // 4 
    {1, 0, 0, 0, 0,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1},

    // 5 
    {1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 0,
     1, 1, 1, 1, 1},

    // 6 
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 0,
     1, 1, 1, 1, 1},

    // 7 
    {0, 0, 0, 0, 1,
     0, 1, 0, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 0, 1, 0,
     1, 1, 1, 1, 1},

    // 8 
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    // 9 
    {1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1}
};

void exibir_numero(int num) {
    // Apaga todos os LEDs da matriz
    for (int i = 0; i < MATRIX_SIZE; i++) {
        leds[i] = 0x000000;  // Todos os LEDs apagados
    }

    // Agora, exibe o novo número
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (numeros[num][i] == 1) {
            // Reduz intensidade multiplicando 1 * O valor definido na intensidade.
            uint8_t r = (uint8_t)(1 * INTENSIDADE);  // Vermelho máximo
            uint8_t g = (uint8_t)(1 * INTENSIDADE);    // Sem verde
            uint8_t b = (uint8_t)(0 * INTENSIDADE);    // Sem azul
            leds[i] = (r << 16) | (g << 8) | b;
        } else {
            leds[i] = 0x000000;  // Apagado
        }
    }

    // Envia os dados para a matriz
    for (int i = 0; i < MATRIX_SIZE; i++) {
        pio_sm_put_blocking(pio, sm, leds[i]);
    }
}


// Callback para tratar o debounce dos botões
bool debounce_callback(struct repeating_timer *t) {
    int pin = (int) t->user_data;

    // Verifica o estado real do botão
    if (gpio_get(pin) == 0) {
        if (pin == BTN_A) {
            numero_atual = (numero_atual + 1) % 10;
        } else if (pin == BTN_B) {
            numero_atual = (numero_atual - 1 + 10) % 10;
        }
        exibir_numero(numero_atual);
    }
    return false;  // Não repete o temporizador
}

// Função chamada quando os botões são pressionados
void gpio_callback(uint gpio, uint32_t events) {
    static struct repeating_timer debounce_timer;
    add_repeating_timer_ms(50, debounce_callback, (void*)gpio, &debounce_timer);
}

// Configuração dos GPIOs e interrupções
void setup() {
    stdio_init_all();

    // Configuração do LED RGB
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);

    // Configuração dos botões com pull-up interno
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);
    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);

    // Configuração da matriz WS2812 com PIO
    uint offset = pio_add_program(pio, &ws2812_program);
    sm = pio_claim_unused_sm(pio, true);
    ws2812_program_init(pio0, sm, offset, MATRIX_PIN, 800000, false);


    // Configuração das interrupções dos botões
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Inicializa o temporizador para piscar o LED RGB | 5 vezes por segundos
    static struct repeating_timer timer;
    add_repeating_timer_ms(200, blink_led_callback, NULL, &timer);
}

int main() {
    setup();

    while (1) {
        sleep_ms(100);  // Reduz consumo de CPU
    }
}
