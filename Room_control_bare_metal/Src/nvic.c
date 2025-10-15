#include "nvic.h"
#include "rcc.h"  // Para rcc_syscfg_clock_enable
#include "uart.h" // Para USART2

static void nvic_enable_irq(uint32_t IRQn)
{
    NVIC->ISER[IRQn / 32U] |= (1UL << (IRQn % 32U));
}

void nvic_set_priority(uint32_t IRQn, uint8_t priority)
{
    NVIC->IP[IRQn] = (priority & 0xFFU);
}

void nvic_exti_pc13_button_enable(void) //Esta función configura el botón conectado al pin PC13 para que genere una interrupción externa (EXTI) cuando se presiona.
{ 
    // 1. Habilitar el reloj para SYSCFG
    rcc_syscfg_clock_enable();

    // 2. Configurar la línea EXTI13 (SYSCFG_EXTICR)
    SYSCFG->EXTICR[3] &= ~(0x000FU << 4);  // Limpiar campo EXTI13
    SYSCFG->EXTICR[3] |=  (0x0002U << 4);  // Conectar EXTI13 a PC13

    // 3. Configurar la línea EXTI13 para interrupción
    EXTI->IMR1 |= (1U << 13);

    // 4. Configurar el trigger de flanco de bajada
    EXTI->FTSR1 |= (1U << 13);
    EXTI->RTSR1 &= ~(1U << 13);

    // 5. Habilitar la interrupción EXTI15_10 en el NVIC
    nvic_enable_irq(EXTI15_10_IRQn);

    // 6. Configurar la prioridad 
    nvic_set_priority(EXTI15_10_IRQn, 1);
}

void nvic_usart2_irq_enable(void)  // Esta función habilita la interrupción del periférico UART2, para reaccionar automáticamente cuando llega un nuevo carácter por serial.
{ 
    // Habilitar interrupción de recepción en USART2
    USART2->CR1 |= (1U << 5);  // RXNEIE
    
    // Habilitar la interrupción USART2 en el NVIC
    nvic_set_priority (USART2_IRQn, 3); 

    nvic_enable_irq(USART2_IRQn);
}