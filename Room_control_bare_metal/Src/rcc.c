#include "rcc.h"
#define RCC_BASE    0x40021000U
#define RCC        ((RCC_TypeDef_t *) RCC_BASE)       // RCC base
//#define RCC_AHB2ENR (*(volatile uint32_t *)(RCC_BASE  + 0x4CU)) // Habilita GPIOA/GPIOC clock

void rcc_init(void)
{
    RCC->AHB2ENR |= (1 << 0);                      // Habilita reloj GPIOA
    RCC->AHB2ENR |= (1 << 1);                      // Habilita reloj GPIOB
    RCC->AHB2ENR |= (1 << 2);                      // Habilita reloj GPIOC
}

void rcc_syscfg_clock_enable(void)
{
    RCC->APB2ENR |= (1 << 0); // Habilita reloj SYSCFG
}

void rcc_tim3_clock_enable(void) // Función para habilitar reloj de TIM3
{
    RCC->APB1ENR1 |= (1 << 1); // TIM3EN = bit 1
}