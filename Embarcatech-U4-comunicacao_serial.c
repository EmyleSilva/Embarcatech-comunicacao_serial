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

//Define a quantidade de LEDS da matriz
#define NUM_PIXELS 25

/**
 * Variaveis globais
 */

ssd1306_t ssd; // Inicializa a estrutura do display
PIO pio = pio0;
uint sm;
bool led_g_active = false;
bool led_b_active = false;
bool display_color = false;
int number_index = 0;

/**
 * Matriz que guarda os frames dos números
 */
const int numbers_frames[10][NUM_PIXELS] = {
    {0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0
    },//0
    {0, 0, 1, 0, 0,
     0, 0, 1, 1, 0,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 1, 1, 1, 0
    },//1
    {0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0
    },//2
    {0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0
    },//3
    {0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 0, 0, 1, 0
    },//4
    {0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0
    },//5
    {0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0
    },//6
    {0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 0, 1, 0,
     0, 0, 0, 0, 0
    },//7
    {0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0
    },//8
    {0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0
    } //9
};



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

uint32_t matrix_rgb(double r, double g, double b)
{
    unsigned char R = (unsigned char)(r * 255);
    unsigned char G = (unsigned char)(g * 255);
    unsigned char B = (unsigned char)(b * 255);

    return (G << 24) | (R << 16) | (B << 8);
} 

/**
 * Exibe um número na matriz de LEDS
 */
void draw_number()
{
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (numbers_frames[number_index][24-i] == 1.0) {
            pio_sm_put_blocking(pio, sm, matrix_rgb(0.5, 0.0, 0.5)); 
        } else {
            pio_sm_put_blocking(pio, sm, matrix_rgb(0.0, 0.0, 0.0));
        }
    }
}

/**
 * Exibe um caracter no display
 */
void draw_display_c(char c)
{
    ssd1306_fill(&ssd, display_color); //Limpa o display
    ssd1306_draw_char(&ssd, c, 64, 32); //Desenha o caracter
    ssd1306_send_data(&ssd); //Envia os dados para o display
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
    
    while (true)
    {
        if (uart_is_readable(UART_ID))
        {
            char c = uart_getc(UART_ID);
            if (c >= '0' && c <= '9')
            {
                number_index = c - 48;
                printf("Numero: %d\n", number_index);
                draw_number();

            }
            draw_display_c(c);
        }
        sleep_ms(500);
    }
}
