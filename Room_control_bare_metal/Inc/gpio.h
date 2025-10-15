// gpio.h
#include <stdint.h>
#ifndef GPIO_H
#define GPIO_H


typedef struct {
    volatile uint32_t MODER; // Mode register
    volatile uint32_t TYPER; // Type register
    volatile uint32_t SPEEDR; // Output speed register
    volatile uint32_t PUPDR; // Pull-up/pull-down register
    volatile uint32_t IDR; // Input data register
    volatile uint32_t ODR; // Output data register
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
    volatile uint32_t BRR;
    volatile uint32_t ASCR;
    
} GPIO_TypeDef_t;

// --- Definiciones de registros para LD2 (Ver RM0351) -------------------------
#define GPIOA_BASE  0x48000000U
#define GPIOA       ((GPIO_TypeDef_t *) GPIOA_BASE)
    
#define GPIOB_BASE  0x48000400U
#define GPIOB       ((GPIO_TypeDef_t *) GPIOB_BASE)// Pin PA5 (LED)

// --- Definiciones de registros para Button B1 (Ver RM0351) ------------------
#define GPIOC_BASE  0x48000800U
#define GPIOC       ((GPIO_TypeDef_t *) GPIOC_BASE)

#define GPIOD_BASE  0x48000C00U
#define GPIOD       ((GPIO_TypeDef_t *) GPIOD_BASE)// Pin PC13

#define GPIOE_BASE  0x48001000U
#define GPIOE       ((GPIO_TypeDef_t *) GPIOE_BASE)

#define GPIOF_BASE  0x48001400U
#define GPIOF       ((GPIO_TypeDef_t *) GPIOF_BASE)

#define GPIOG_BASE  0x48001800U
#define GPIOG       ((GPIO_TypeDef_t *) GPIOG_BASE)

#define GPIOH_BASE  0x48001C00U
#define GPIOH       ((GPIO_TypeDef_t *) GPIOH_BASE)

// Pines específicos
#define LD2_Pin        5
#define LD2_GPIO_Port  GPIOA

#define B1_Pin         13
#define B1_GPIO_Port   GPIOC

// Configuraciones de modo GPIO
#define GPIO_MODE_INPUT   0x00
#define GPIO_MODE_OUTPUT  0x01
#define GPIO_MODE_AF      0x02
#define GPIO_MODE_ANALOG  0x03

#define GPIO_OTYPE_PP     0x00
#define GPIO_OTYPE_OD     0x01

#define GPIO_OSPEED_LOW   0x00
#define GPIO_OSPEED_MED   0x01
#define GPIO_OSPEED_HIGH  0x02
#define GPIO_OSPEED_VHIGH 0x03

#define GPIO_PUPD_NONE    0x00
#define GPIO_PUPD_PU      0x01
#define GPIO_PUPD_PD      0x02


void gpio_init_pin(GPIO_TypeDef_t *GPIO, uint8_t pin, uint8_t mode, uint8_t type, uint8_t speed, uint8_t pupd);
void set_gpio(GPIO_TypeDef_t *GPIO, uint8_t pin);
void clear_gpio(GPIO_TypeDef_t *GPIO, uint8_t pin);
uint8_t read_gpio(GPIO_TypeDef_t *GPIO, uint8_t pin);
void gpio_toggle_ld2(void);
void led_on(void);
void led_off(void);

#endif