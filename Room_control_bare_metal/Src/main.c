#include "gpio.h"      // llama a las funciones de perifericos y la logica del control de sala
#include "systick.h"   // la logica del control de sala
#include "rcc.h"
#include "uart.h"
#include "nvic.h"
#include "tim.h"
#include "room_control.h"

// Flags para eventos
volatile uint8_t button_event = 0; // indoca que se ha detectado una pulsacion de boton en la interrupcion 
volatile char uart_event_char = 0; // volatile ya que es crucial porque se comparte entre la interrupción y el bucle principal.
                                   // char almacena el dato - caracter recibido por uart en la interrupcion
                                   
// Función local para inicializar periféricos
static void peripherals_init(void)
{
    rcc_init();  // Inicialización del sistema de reloj
    
    // Configuración de GPIOs
    gpio_init_pin(GPIOA, 5, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO_OSPEED_LOW, GPIO_PUPD_NONE);
    gpio_init_pin(GPIOB, 3, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO_OSPEED_LOW, GPIO_PUPD_NONE);
    gpio_init_pin(GPIOC, 13, GPIO_MODE_INPUT, GPIO_OTYPE_PP, GPIO_OSPEED_LOW, GPIO_PUPD_PU); // Botón con pull-up



    // Inicialización de periféricos
    init_systick();
    init_gpio_uart();
    init_uart();  // Asumiendo función unificada
    nvic_exti_pc13_button_enable();
    nvic_usart2_irq_enable();
    tim3_ch1_pwm_init(1000);  // 1 kHz PWM
}

int main(void)
{
    peripherals_init(); 
    room_control_app_init();
    uart_send_string("Sistema de Control de Sala Inicializado!\r\n"); //esto lo hace dos veces 

    // Bucle principal: procesa eventos
    while (1) {
        if (button_event) {
            button_event = 0;
            room_control_on_button_press();
        }         
        if (uart_event_char) {
            char c = uart_event_char;
            uart_event_char = 0;
            room_control_on_uart_receive(c);
        }
        // Llamar a la función de actualización periódica
        room_control_update();
    }
}

// Manejadores de interrupciones
void EXTI15_10_IRQHandler(void)
{
    // Limpiar flag de interrupción
    if (EXTI->PR1 & (1 << 13)) {
        EXTI->PR1 |= (1 << 13);  // Clear pending
        button_event = 1;
    }
}

void USART2_IRQHandler(void)
{
    // Verificar si es recepción
    if (USART2->ISR & (1 << 5)) {  // RXNE
        uart_event_char = (char)(USART2->RDR & 0xFF);
    }
}