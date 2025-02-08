#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/uart.h"
#include "hardware/clocks.h"
#include "pio_matrix.pio.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

//Definição de entradas/saidas conectadas as GPIOs
#define MATRIX 7
#define BUTTON_A 5
#define BUTTON_B 6
#define green_led_pin 11
#define blue_led_pin 12

//Definições do I2C
#define I2C_PORT i2c1
#define I2C_SDA 14 //SDA
#define I2C_SCL 15 //SLC
#define address 0x3C

// Definições UART
#define UART_ID uart0
#define BAUD_RATE 115200


/**
 * Variaveis globais
 */
PIO pio = pio0;
uint sm;


void init_leds ()
{
    gpio_init(green_led_pin);
    gpio_set_dir(green_led_pin, GPIO_OUT);

    gpio_init(blue_led_pin);
    gpio_set_dir(blue_led_pin, GPIO_OUT);
}

void init_buttons ()
{
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
}

int main()
{
    stdio_init_all();
    init_leds();
    init_buttons();

    /**
     * Configuração do I2C
     */
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    /**
     * Configuração do display
     */
    ssd1306_t ssd; // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, address, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display
    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
   
    /**
     * Configuração do UART
     */
    uart_init(uart0, BAUD_RATE);

    /**
     * Configuração do PIO para matriz de LEDS
     */
    bool ok;
    ok = set_sys_clock_khz(128000, false);
    if (ok) printf("Clock set to: %ld\n\n", clock_get_hz(clk_sys));    
 
    uint offset = pio_add_program(pio, &pio_matrix_program);
    sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, MATRIX);
    
    bool cor = true;
    while (true)
    {
        cor = !cor;
        // Atualiza o conteúdo do display com animações
        //ssd1306_fill(&ssd, cor); // Limpa o display
        //ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
        ssd1306_draw_string(&ssd, "r s t u v", 8, 10); // Desenha uma string
        ssd1306_draw_string(&ssd, "w x y z", 8, 30); // Desenha uma string
        ssd1306_draw_string(&ssd, "PROF WILTON", 8, 48); // Desenha uma string      
        ssd1306_send_data(&ssd); // Atualiza o display

        sleep_ms(1000);
    }
}
