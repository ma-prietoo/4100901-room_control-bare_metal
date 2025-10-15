#include "tim.h"
#include "rcc.h"  // Para rcc_tim3_clock_enable
#include "gpio.h" // Para gpio_setup_pin

void tim3_ch1_pwm_init(uint32_t pwm_freq_hz)
{
    // 1. Configurar PA6 como Alternate Function (AF2) para TIM3_CH1
    gpio_init_pin(GPIOA, 6, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_MED, GPIO_PUPD_NONE);
    GPIOA->AFRL &= ~((0xFU<<24));
    GPIOA->AFRL |=  ((2U<<24));  // AF2
    // 2. Habilitar el reloj para TIM3
    rcc_tim3_clock_enable();

    // 3. Configurar TIM3
    TIM3->PSC = 100 - 1; // (4MHz / 100 = 40kHz)
    TIM3->ARR = (TIM_PCLK_FREQ_HZ / 100 / pwm_freq_hz) - 1; // 40kHz / pwm_freq_hz

    // 4. Configurar el Canal 1 (CH1) en modo PWM 1
    TIM3->CCMR1 |= (6U << 4) | (1U << 3);  // PWM1 + preload enable
    TIM3->CCER  |= (1U << 0);              // Enable CH1 output
    TIM3->EGR   |= (1U << 0);              // Generate update
    TIM3->CR1   |= (1U << 0);              // Enable counter

}

void tim3_ch1_pwm_set_duty_cycle(uint8_t duty_cycle_percent)
{
    if (duty_cycle_percent > 100) {
        duty_cycle_percent = 100;
    }

    // Calcular el valor de CCR1 basado en el porcentaje y el valor de ARR
    uint16_t tim3_ch1_arr_value = TIM3->ARR;
    uint32_t ccr_value = (((uint32_t)tim3_ch1_arr_value + 1U) * duty_cycle_percent) / 100U;

    TIM3->CCR1 = ccr_value;
}