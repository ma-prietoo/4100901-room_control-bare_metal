// uart.h
#include <stdint.h>
#include "gpio.h"  // Librería GPIO existente
#include "rcc.h"   // Librería RCC existente

// USART2
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    volatile uint32_t GTPR;
    volatile uint32_t RTOR;
    volatile uint32_t RQR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t RDR;
    volatile uint32_t TDR;
} USART_Typedef_t;
#define USART2 ((USART_Typedef_t *)0x40004400U)

// Constantes
#define BAUD_RATE     115200U
#define HSI_FREQ      4000000U

// Prototipos de funciones
void init_gpio_uart(void);
void init_uart(void);
void uart_send(char c);
char uart_receive(void);
void uart_send_string(const char *str);