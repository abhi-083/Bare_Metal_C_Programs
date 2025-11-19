#include <stdint.h>

#define PERIPH_BASE (0x40000000UL)
#define IOPORT_OFFSET (0x10000000UL)
#define IOPORT_BASE (PERIPH_BASE + IOPORT_OFFSET)
#define GPIOA_OFFSET (0x0000UL)
#define GPIOA_BASE (IOPORT_BASE + GPIOA_OFFSET)
#define GPIOC_OFFSET (0x800UL)
#define GPIOC_BASE (IOPORT_BASE + GPIOC_OFFSET)
#define AHBPERIPH_OFFSET (0x00020000UL)
#define AHBPERIPH_BASE (PERIPH_BASE + AHBPERIPH_OFFSET)
#define RCC_OFFSET (0x1000UL)
#define RCC_BASE (AHBPERIPH_BASE + RCC_OFFSET)
#define IOPORTEN_R_OFFSET (0x34UL)
#define RCC_IOPORTEN_R (*(volatile unsigned int *)(RCC_BASE + IOPORTEN_R_OFFSET))
#define GPIOAEN (1U<<0)
#define GPIOCEN (1U<<2)
#define MODE_R_OFFSET (0x00UL)
#define GPIOA_MODE_R (*(volatile unsigned int *)(GPIOA_BASE + MODE_R_OFFSET))
#define GPIOC_MODE_R (*(volatile unsigned int *)(GPIOC_BASE + MODE_R_OFFSET))
#define OD_R_OFFSET (0x14UL)
#define ID_R_OFFSET (0x10UL)
#define GPIOA_OD_R (*(volatile unsigned int *)(GPIOA_BASE + OD_R_OFFSET))
#define GPIOC_ID_R (*(volatile unsigned int *)(GPIOC_BASE + ID_R_OFFSET))
#define PIN5 (1U<<5)
#define LED_PIN PIN5
#define PINC13 (1U<<13)
#define USER_BUTTON PINC13

int main(void)
{
	RCC_IOPORTEN_R |= GPIOAEN;
	RCC_IOPORTEN_R |= GPIOCEN;

	//	00: Input (reset state)
	//	01: General purpose output mode
	//	10: Alternate function mode
	//	11: Analog mode

	// PA5 in output mode
	GPIOA_MODE_R |= (1U<<10);
	GPIOA_MODE_R &= ~(1U<<11);

	// PC13 in input mode / UserButton is in active_low state
	GPIOC_MODE_R &= ~(1U<<26);
	GPIOC_MODE_R &= ~(1U<<27);

	uint8_t last = (GPIOC_ID_R & USER_BUTTON) ? 1 : 0;

	while(1)
	{
		uint8_t cur = (GPIOC_ID_R & USER_BUTTON) ? 1 : 0;

		// For active LOW button:
		// Released = 1, Pressed = 0
		if (last == 1 && cur == 0)  // detect falling edge (press)
		{
		     GPIOA_OD_R ^= LED_PIN;   // toggle LED
		}

		last = cur;
	}
}
