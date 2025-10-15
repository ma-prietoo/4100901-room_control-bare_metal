// rcc.h
#include <stdint.h>
#ifndef RCC_H
#define RCC_H

typedef struct {
    volatile uint32_t CR;            // (0x00) Control register
    volatile uint32_t ICSCR;         // (0x04) Internal clock sources calibration register
    volatile uint32_t CFGR;          // (0x08) Clock configuration register
    volatile uint32_t PLLCFGR;       // (0x0C) PLL configuration register
    volatile uint32_t PLLSAI1CFGR;   // (0x10) PLLSAI1 configuration register
    volatile uint32_t PLLSAI2CFGR;   // (0x14) PLLSAI2 configuration register
    volatile uint32_t CIER;          // (0x18) Clock interrupt enable register
    volatile uint32_t CIFR;          // (0x1C) Clock interrupt flag register
    volatile uint32_t CICR;          // (0x20) Clock interrupt clear register
    volatile uint32_t RESERVED1;     // (0x24) Reserved
    volatile uint32_t AHB1RSTR;      // (0x28) AHB1 peripheral reset register
    volatile uint32_t AHB2RSTR;      // (0x2C) AHB2 peripheral reset register
    volatile uint32_t AHB3RSTR;      // (0x30) AHB3 peripheral reset register
    volatile uint32_t RESERVED2;     // (0x34) Reserved
    volatile uint32_t APB1RSTR1;     // (0x38) APB1 peripheral reset register 1
    volatile uint32_t APB1RSTR2;     // (0x3C) APB1 peripheral reset register 2
    volatile uint32_t APB2RSTR;      // (0x40) APB2 peripheral reset register
    volatile uint32_t RESERVED3;     // (0x44) Reserved
    volatile uint32_t AHB1ENR;       // (0x48) AHB1 peripheral clock enable register
    volatile uint32_t AHB2ENR;       // (0x4C) AHB2 peripheral clock enable register
    volatile uint32_t AHB3ENR;       // (0x50) AHB3 peripheral clock enable register
    volatile uint32_t RESERVED4;     // (0x54) Reserved
    volatile uint32_t APB1ENR1;      // (0x58) APB1 peripheral clock enable register 1
    volatile uint32_t APB1ENR2;      // (0x5C) APB1 peripheral clock enable register 2
    volatile uint32_t APB2ENR;       // (0x60) APB2 peripheral clock enable register
    volatile uint32_t RESERVED5;     // (0x64) Reserved
    volatile uint32_t AHB1SMENR;     // (0x68)
    volatile uint32_t AHB2SMENR;     // (0x6C) AHB2 peripheral clock enable in Sleep and Stop modes register
    volatile uint32_t AHB3SMENR;     // (0x70)
    volatile uint32_t RESERVED6;     // (0x74) Reserved
    volatile uint32_t APB1SMENR1;    // (0x78
    volatile uint32_t APB1SMENR2;    // (0x7C)
    volatile uint32_t APB2SMENR;     // (0x80)
    volatile uint32_t RESERVED7;     // (0x84) Reserved
    volatile uint32_t CCIPR;         // (0x88) Peripherals independent
    volatile uint32_t RESERVED8;     // (0x8C) Reserved
    volatile uint32_t BDCR;          // (0x90) Backup domain control register
    volatile uint32_t CSR;           // (0x94) Control/status register
} RCC_TypeDef_t;

#define RCC_BASE    0x40021000U
#define RCC         ((RCC_TypeDef_t *) RCC_BASE)

void rcc_init(void);
void rcc_syscfg_clock_enable(void);
#endif // RCC_H