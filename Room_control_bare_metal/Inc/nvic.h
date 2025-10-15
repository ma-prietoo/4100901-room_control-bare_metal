#ifndef NVIC_H
#define NVIC_H

#include <stdint.h>

// SYSCFG
typedef struct {
    volatile uint32_t MEMRMP;
    volatile uint32_t CFGR1;
    volatile uint32_t EXTICR[4];
    volatile uint32_t SCSR;
    volatile uint32_t CFGR2;
    volatile uint32_t SWPR;
    volatile uint32_t SKR;
} SYSCFG_TypeDef;

// EXTI
typedef struct {
    volatile uint32_t IMR1;
    volatile uint32_t EMR1;
    volatile uint32_t RTSR1;
    volatile uint32_t FTSR1;
    volatile uint32_t SWIER1;
    volatile uint32_t PR1;
    uint32_t RESERVED1[2];
    volatile uint32_t IMR2;
    volatile uint32_t EMR2;
    volatile uint32_t RTSR2;
    volatile uint32_t FTSR2;
    volatile uint32_t SWIER2;
    volatile uint32_t PR2;
} EXTI_TypeDef;

// NVIC
typedef struct {
    volatile uint32_t ISER[8U];         /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register */
    uint32_t RESERVED0[24U];
    volatile uint32_t ICER[8U];         /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register */
    uint32_t RESERVED1[24U];
    volatile uint32_t ISPR[8U];         /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register */
    uint32_t RESERVED2[24U];
    volatile uint32_t ICPR[8U];         /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register */
    uint32_t RESERVED3[24U];
    volatile uint32_t IABR[8U];         /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register */
    uint32_t RESERVED4[56U];
    volatile uint8_t  IP[240U];         /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
    uint32_t RESERVED5[644U];
    volatile uint32_t STIR;             /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register */
} NVIC_Type;

#define SYSCFG_BASE         (0x40010000UL)
#define EXTI_BASE           (0x40010400UL)
#define NVIC_BASE           (0xE000E100UL)

#define SYSCFG              ((SYSCFG_TypeDef *) SYSCFG_BASE)
#define EXTI                ((EXTI_TypeDef *)   EXTI_BASE)
#define NVIC                ((NVIC_Type *)      NVIC_BASE)

// IRQn enumerations (extracto para STM32L476RG)
typedef enum {
    EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                  */
    USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                          */
} IRQn_Type;

// Constantes
#define RCC_APB2ENR_SYSCFGEN_Pos    (0U)
#define RCC_APB2ENR_SYSCFGEN_Msk    (0x1UL << RCC_APB2ENR_SYSCFGEN_Pos)
#define RCC_APB2ENR_SYSCFGEN        RCC_APB2ENR_SYSCFGEN_Msk

// Prototipos de funciones
void nvic_exti_pc13_button_enable(void); // Configura EXTI13 y habilita su IRQ en NVIC
void nvic_usart2_irq_enable(void);   
void rcc_syscfg_clock_enable(void);    // Habilita el reloj para SYSCFG

#endif // NVIC_H