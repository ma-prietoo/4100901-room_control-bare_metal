#ifndef TIM_H
#define TIM_H

#include <stdint.h>
#include "gpio.h"  // Librería GPIO existente
#include "rcc.h"   // Librería RCC existente

// TIM3
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RESERVED1;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t RESERVED2;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
} TIM_TypeDef;

#define TIM3_BASE           (0x40000400UL)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)

// Constantes
#define TIM_PCLK_FREQ_HZ    4000000U   // 4 MHz APB1 timer clock
#define PWM_FREQUENCY       1000U      // 1 kHz PWM
#define PWM_PERIOD          (TIM_PCLK_FREQ_HZ / PWM_FREQUENCY)
#define PWM_DUTY_CYCLE      50         // 50% duty cycle
#define PWM_PIN             6U         // PA6 = TIM3_CH1

#define PWM_DC_TO_CCR(DC) ((PWM_PERIOD * (DC)) / 100) // Macro para calcular CCR

// Prototipos de funciones
void tim3_ch1_pwm_init(uint32_t pwm_freq_hz);
void tim3_ch1_pwm_set_duty_cycle(uint8_t duty_cycle_percent); // duty_cycle en % (0-100)

#endif // TIM_H