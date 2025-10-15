#include <stdint.h>
#include "systick.h"

// --- Definiciones de registros para SysTick (Ver PM0214)  --------------------
#define SYST_BASE   0xE000E010U  
#define SYST       ((SysTick_TypeDef_t *) SYST_BASE)                               // SysTick base
//#define SYST_CSR    (*(volatile uint32_t *)(SYST_BASE + 0x00U)) // Control y estado
//#define SYST_RVR    (*(volatile uint32_t *)(SYST_BASE + 0x04U)) // Valor de recarga
#define HSI_FREQ    4000000U     

// Contador global de milisegundos
static volatile uint32_t ms_ticks = 0;// Reloj interno 4 MHz

void init_systick(void)
{
  SYST->RVR = HSI_FREQ / 1000 - 1;
  SYST->CVR = 0;                               // Reiniciar el contador
  SYST->CSR = (1 << 0) | (1 << 1) | (1 << 2);  // ENABLE|TICKINT|CLKSOURCE
}

uint32_t systick_get_ms(void)
{
    return ms_ticks;
}

void SysTick_Handler(void) {
    ms_ticks++;
}