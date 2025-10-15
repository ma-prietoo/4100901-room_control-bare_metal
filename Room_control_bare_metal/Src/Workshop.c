// --- Ejemplo de parpadeo de LED LD2 en STM32L476RGTx -------------------------
#include <stdint.h>

#define RCC_BASE    0x40021000U
#define RCC_AHB2ENR (*(volatile uint32_t *)(RCC_BASE  + 0x4CU)) // Habilita GPIOA/GPIOC clock
// --- Definiciones de registros para LD2 (Ver RM0351) -------------------------


#define GPIOA_BASE  0x48000000U
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00U)) // Configuración de modo
#define GPIOA_ODR   (*(volatile uint32_t *)(GPIOA_BASE + 0x14U)) // Data de salida
#define LD2_PIN     5U                                         // Pin PA5 (LED)

// --- Definiciones de registros para Button B1 (Ver RM0351) ------------------
#define GPIOC_BASE  0x48000800U
#define GPIOC_MODER (*(volatile uint32_t *)(GPIOC_BASE + 0x00U)) // Configuración de modo
#define GPIOC_IDR   (*(volatile uint32_t *)(GPIOC_BASE + 0x10U)) // Data de entrada
#define B1_PIN      13U                                        // Pin PC13 (Button)

// --- Definiciones de registros para SysTick (Ver PM0214)  --------------------
#define SYST_BASE   0xE000E010U                                 // SysTick base
#define SYST_CSR    (*(volatile uint32_t *)(SYST_BASE + 0x00U)) // Control y estado
#define SYST_RVR    (*(volatile uint32_t *)(SYST_BASE + 0x04U)) // Valor de recarga
#define HSI_FREQ    4000000U                                   // Reloj interno 4 MHz

#define WFI()       __asm volatile("wfi")

// --- Variable global para contar segundos -----------------------------------
static volatile uint32_t seconds_counter = 17;

void init_systick(void);
void init_led(void);
void init_button(void);
void check_button(void);

// --- Programa principal ------------------------------------------------------
int main(void)
{
    init_led();
    init_button();
    init_systick();
    while (1) {
        check_button();
        WFI();  // Espera interrupción
    }
}

// --- Inicialización de GPIOA PA5 para el LED LD2 -----------------------------
void init_led(void)
{
    RCC_AHB2ENR |= (1 << 0);                      // Habilita reloj GPIOA
    GPIOA_MODER &= ~(3 << (LD2_PIN*2));           // Limpia bits
    GPIOA_MODER |=  (1 << (LD2_PIN*2));           // Configura como salida
}

// --- Inicialización de GPIOC PC13 para el botón B1 ---------------------------
void init_button(void)
{
    RCC_AHB2ENR |= (1 << 2);                      // Habilita reloj GPIOC
    GPIOC_MODER &= ~(3 << (B1_PIN*2));            // PC13 como entrada (00)
}

// --- Verificación del botón y control del LED --------------------------------
void check_button(void)
{
    // Leer estado del botón PC13 (botón presionado = 0, no presionado = 1)
    if (!(GPIOC_IDR & (1 << B1_PIN))) {
        // Botón presionado: reiniciar contador y encender LED
        seconds_counter = 0;
        GPIOA_ODR |= (1 << LD2_PIN);              // Encender LED
    }
}

// --- Inicialización de Systick para 1 s --------------------------------------
void init_systick(void)
{
    SYST_RVR = HSI_FREQ / 1000 - 1;                      // Recarga = 4000 - 1
    SYST_CSR = (1 << 0) | (1 << 1) | (1 << 2);    // ENABLE|TICKINT|CLKSOURCE
}

// --- Manejador de la interrupción SysTick ------------------------------------
void SysTick_Handler(void)
{
    // Incrementar contador de segundos
    seconds_counter++;
    
    // Si han pasado 3 segundos o más, apagar LED
    if (seconds_counter >= 3000) {
        GPIOA_ODR &= ~(1 << LD2_PIN);             // Apagar LED
    }
}