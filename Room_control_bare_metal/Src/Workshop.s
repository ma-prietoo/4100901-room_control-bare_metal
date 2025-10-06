// --- Ejemplo Botón y LED con temporización -------------------------
    .section .text
    .syntax unified
    .thumb

    .global main
    .global init_led
    .global init_button
    .global init_systick
    .global SysTick_Handler

// --- Definir PA5 ----------------------
    .equ RCC_BASE,       0x40021000
    .equ RCC_AHB2ENR,    RCC_BASE + 0x4C
    .equ GPIOA_BASE,     0x48000000
    .equ GPIOA_MODER,    GPIOA_BASE + 0x00
    .equ GPIOA_ODR,      GPIOA_BASE + 0x14
    .equ LD2_PIN,        5

// --- Definir PC13 ----------------
    .equ GPIOC_BASE,     0x48000800
    .equ GPIOC_MODER,    GPIOC_BASE + 0x00
    .equ GPIOC_IDR,      GPIOC_BASE + 0x10
    .equ BTN_PIN,        13

// --- Definir SysTick (reloj)---------------------------------------
    .equ SYST_CSR,       0xE000E010
    .equ SYST_RVR,       0xE000E014
    .equ SYST_CVR,       0xE000E018
    .equ HSI_FREQ,       4000000

// --- Variables en RAM ----------------------------------------------
    .section .bss
    .align 4
led_timer:      .skip 4       @ tiempo restante LED encendido

// --- Programa principal --------------------------------------------
    .section .text
main:
    bl init_led
    bl init_button
    bl init_systick

loop:
    bl read_button
    wfi
    b loop

// --- Inicialización de LED con PA5 como salida ----------------------------
init_led:
    movw  r0, #:lower16:RCC_AHB2ENR        @ Cargar mitad baja de la dirección RCC_AHB2ENR en r0
    movt  r0, #:upper16:RCC_AHB2ENR        @ Cargar mitad alta y ahora  r0 contiene la dirección completa
    ldr   r1, [r0]
    orr   r1, r1, #(1 << 0)                @ GPIOA SE ACTIVA CON UN 1 EN EL BIT 0
    str   r1, [r0]

    movw  r0, #:lower16:GPIOA_MODER        
    movt  r0, #:upper16:GPIOA_MODER        
    ldr   r1, [r0]
    bic   r1, r1, #(0b11 << (LD2_PIN * 2)) @Limpia los dos bits y los deja 00
    orr   r1, r1, #(0b01 << (LD2_PIN * 2)) @ PA5 salida CON UN 01 para que sea una salida
    str   r1, [r0]
    bx    lr

// --- Inicialización del botón (PC13 entrada) -----------------------
init_button:
    movw  r0, #:lower16:RCC_AHB2ENR
    movt  r0, #:upper16:RCC_AHB2ENR
    ldr   r1, [r0]
    orr   r1, r1, #(1 << 2)                @ GPIOC SE HABILITA CON UN 1 EN SU BIT 2
    str   r1, [r0]

    movw  r0, #:lower16:GPIOC_MODER
    movt  r0, #:upper16:GPIOC_MODER
    ldr   r1, [r0]
    bic   r1, r1, #(0b11 << (BTN_PIN * 2)) @ los limpua y queda PC13 como entrada con 00
    str   r1, [r0]
    bx    lr

// --- Inicialización SysTick para 1 ms -------------------------------
init_systick:
    movw  r0, #:lower16:SYST_RVR
    movt  r0, #:upper16:SYST_RVR
    ldr   r1, =3999                         @ 4 MHz / 4000 = 1 ms
    str   r1, [r0]

    movw  r0, #:lower16:SYST_CSR
    movt  r0, #:upper16:SYST_CSR
    movs  r1, #(1<<0)|(1<<1)|(1<<2)         @ ENABLE, TICKINT, CLKSOURCE
    str   r1, [r0]
    bx    lr

// --- Leer botón y encender LED si se presiona ----------------------
read_button:
    movw  r0, #:lower16:GPIOC_IDR
    movt  r0, #:upper16:GPIOC_IDR
    ldr   r1, [r0]
    tst   r1, #(1 << BTN_PIN)               @ MIRA SI EN EL PIN 13 HAY UN  1 = BOTON PRESIONADO
    bne   no_press                          @ SI NO ESTA PRESIONADO NO HACE NADA

    @ AHORA ENCENDEMOS EL LED
    movw  r0, #:lower16:GPIOA_ODR
    movt  r0, #:upper16:GPIOA_ODR
    ldr   r1, [r0]
    orr   r1, r1, #(1 << LD2_PIN)    @EL PIN 5 QUE MANEJA EL LED SE COLOCA EN 1 PARA ENCENDER EL LED
    str   r1, [r0]

    @ Cargar temporizador LED = 3000 ms
    ldr   r0, =led_timer
    ldr   r1, =3000
    str   r1, [r0]                  @ Escribir 3000 en led_timer Y la ISR SysTick lo irá decrementando cada ms

no_press:
    bx lr

// --- SysTick Handler: decrementa temporizador ----------------------
    .thumb_func
SysTick_Handler:
    @ Decrementar temporizador LED si > 0
    ldr   r0, =led_timer
    ldr   r1, [r0]
    cmp   r1, #0
    beq   end_tick
    subs  r1, r1, #1
    str   r1, [r0]
    cmp   r1, #0
    bne   end_tick

    @ Apagar LED cuando llegue a 0
    movw  r0, #:lower16:GPIOA_ODR
    movt  r0, #:upper16:GPIOA_ODR
    ldr   r2, [r0]
    bic   r2, r2, #(1 << LD2_PIN)     @LIMPIA EL LED PARA APAGARLO
    str   r2, [r0]

end_tick:
    bx lr