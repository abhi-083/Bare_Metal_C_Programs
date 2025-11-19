#include <stdint.h>

#define PERIPH_BASE        (0x40000000UL)
#define IOPORT_BASE        (0x50000000UL)   /* I/O port registers base (GPIOA..GPIOZ) */
#define GPIOA_BASE         (IOPORT_BASE + 0x0000UL)

#define RCC_BASE           (0x40021000UL)   /* RCC base for STM32G0 series */
#define RCC_IOPENR_OFFSET  (0x34UL)         /* IOPENR / IOPORT enable register offset */

#define RCC_IOPENR         (*(volatile uint32_t *)(RCC_BASE + RCC_IOPENR_OFFSET))

#define GPIO_MODER_OFFSET  (0x00UL)
#define GPIO_ODR_OFFSET    (0x14UL)

#define GPIOA_MODER        (*(volatile uint32_t *)(GPIOA_BASE + GPIO_MODER_OFFSET))
#define GPIOA_ODR          (*(volatile uint32_t *)(GPIOA_BASE + GPIO_ODR_OFFSET))

#define GPIOAEN            (1U << 0)        /* IOPAEN bit in RCC_IOPENR */
#define PIN5               (1U << 5)
#define LED_PIN            PIN5

static void delay(volatile uint32_t d) { while (d--) { __asm__ volatile("nop"); } }

int main(void)
{
    /* Enable GPIOA clock */
    RCC_IOPENR |= GPIOAEN;
    GPIOA_MODER |=  (1U << 10);
    GPIOA_MODER &= ~(1U << 11);

    while (1)
    {
        GPIOA_ODR ^= LED_PIN;    /* toggle PA5 */
        delay(300000);
    }
}
