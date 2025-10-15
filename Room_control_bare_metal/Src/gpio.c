#include "gpio.h"
   
// Define LD2_PIN with the correct pin number (e.g., 5 for PA5 on STM32 boards)
#define LD2_PIN 5
   

void gpio_init_pin(GPIO_TypeDef_t * GPIO, uint8_t pin, uint8_t mode, uint8_t type, uint8_t speed, uint8_t pupd)
{
    // Configurar modo
    GPIO->MODER &= ~(0x3U << (2 * pin));
    GPIO->MODER |= (mode << (2 * pin));

    // Configurar tipo
    GPIO->TYPER &= ~(1U << pin);
    GPIO->TYPER |= (type << pin);

    // Configurar velocidad
    GPIO->SPEEDR &= ~(0x3U << (2 * pin));
    GPIO->SPEEDR |= (speed << (2 * pin));

    // Configurar pull-up/pull-down
    GPIO->PUPDR &= ~(0x3U << (2 * pin));
    GPIO->PUPDR |= (pupd << (2 * pin));
}


void set_gpio(GPIO_TypeDef_t * GPIO, uint8_t pin)
{
  GPIO->ODR |= (1 << pin);              // Encender LED
}

void clear_gpio(GPIO_TypeDef_t * GPIO, uint8_t pin)
{
  GPIO->ODR &= ~(1 << pin);             // Apagar LED
}

uint8_t read_gpio(GPIO_TypeDef_t * GPIO, uint8_t pin)
{
// Leer estado del botón PC13 (botón presionado = 0, no presionado = 1)
    if ((GPIO->IDR & (1 << pin))==0) { // presionado
        return 1; // Botón presionado
    }
        return 0; // Botón no presionado
}

void led_on(void)
{
    set_gpio(GPIOA, 5);
}

void led_off(void)
{
    clear_gpio(GPIOA, 5);
}

void gpio_toggle_ld2(void) // Cambia el estado del LED LD2 conectado al pin PA5.
                           // Es decir, si está encendido, lo apaga; si está apagado, lo enciende.
{
    GPIOA->ODR ^= (1 << 5);
}