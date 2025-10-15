#include "room_control.h"
#include "gpio.h"    // Para controlar LEDs
#include "systick.h" // Para obtener ticks y manejar tiempos
#include "uart.h"    // Para enviar mensajes
#include "tim.h"     // Para controlar el PWM

// Prototipo para evitar declaración implícita
void gpio_toggle_ld2(void);

// Estados de la sala
typedef enum {
    ROOM_IDLE,
    ROOM_OCCUPIED
} room_state_t;

// Variable de estado global
room_state_t current_state = ROOM_IDLE;
static uint32_t last_button_press_time = 0;
static uint32_t led_on_time = 0;
static uint8_t pwm_duty = PWM_INITIAL_DUTY;

// Variables para debounce

#define DEBOUNCE_DELAY_MS 200  // 200 ms de filtro anti-rebote


void room_control_app_init(void)
{
    led_off();
    current_state = ROOM_IDLE;
    uart_send_string("Room Control Initialized\r\n");
}

void room_control_on_button_press(void)
{
    uint32_t now = systick_get_ms();

    // Anti-rebote: ignora eventos muy seguidos
    if (now - last_button_press_time < DEBOUNCE_DELAY_MS)
        return;

    last_button_press_time = now;

    if (current_state == ROOM_IDLE)
    {
        current_state = ROOM_OCCUPIED;
        led_on();
        uart_send_string("Estado: OCCUPIED\r\n");
    }
    else
    {
        current_state = ROOM_IDLE;
        led_off();
        uart_send_string("Estado: IDLE\r\n");
    }
}

void room_control_on_uart_receive(char received_char) // NO ENTIENDO QUE HACE ESTA FUNCION
{
    static uint8_t awaiting_pwm_digit = 0;

    // --- Si el comando anterior fue 'B', esperamos un dígito ---
    if (awaiting_pwm_digit && received_char >= '0' && received_char <= '9')
    {
        uint8_t new_duty = (received_char - '0') * 10;  // 'B5' = 50%
        tim3_ch1_pwm_set_duty_cycle(new_duty);
        pwm_duty = new_duty;
        awaiting_pwm_digit = 0;

        uart_send_string("PWM duty cycle");
        uint8_t buffer [2]={received_char, 0};
        uart_send_string(buffer);
        
        return;
    }

    switch (received_char)
    {
        case 'B':
        case 'b':
            uart_send_string("Comando B recibido. Envie un número (0-9) para el duty.\r\n");
            awaiting_pwm_digit = 1;
            break;

        case 'H':
        case 'h':
            tim3_ch1_pwm_set_duty_cycle(100);
            pwm_duty = 100;
            uart_send_string("PWM al 100%\r\n");
            break;

        case 'L':
        case 'l':
            tim3_ch1_pwm_set_duty_cycle(0);
            pwm_duty = 0;
            uart_send_string("PWM al 0%\r\n");
            break;

        case 'O':
        case 'o':
            current_state = ROOM_OCCUPIED;
            led_on();
            tim3_ch1_pwm_set_duty_cycle(100);
            uart_send_string("Sala ocupada\r\n");
            led_on_time = systick_get_ms();
            break;

        case 'I':
        case 'i':
            current_state = ROOM_IDLE;
            led_off();
            tim3_ch1_pwm_set_duty_cycle(0);
            uart_send_string("Sala vacía\r\n");
            break;

        default:
            // Echo: devuelve el carácter recibido
            //uart_send_string("received_char");
            break;
    }
}

void room_control_update(void)
{
    if (current_state == ROOM_OCCUPIED) {
        uint32_t now = systick_get_ms();
        if (now - led_on_time >= LED_TIMEOUT_MS) {
            current_state = ROOM_IDLE;
            led_off();
            tim3_ch1_pwm_set_duty_cycle(0);
            uart_send_string("Timeout: sala vacia\r\n");
        }
    }
}