// systick.h
#include <stdint.h>

typedef struct {
    volatile uint32_t CSR; // Control and status register
    volatile uint32_t RVR; // Reload value register
    volatile uint32_t CVR; // Current value register
    volatile uint32_t CALIB; // Calibration register
} SysTick_TypeDef_t;

#define SYSTICK_BASE 0xE000E010U
#define SYSTICK      ((SysTick_TypeDef_t *) SYSTICK_BASE)

void init_systick(void); // Inicializa el SysTick

uint32_t systick_get_ms(void); // Devuelve el tiempo en ms desde el inicio