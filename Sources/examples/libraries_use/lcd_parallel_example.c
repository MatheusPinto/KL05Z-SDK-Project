#include "mcu/drivers/port/port.h"
#include "mcu/drivers/gpio/gpio.h"
#include "libraries/lcd/adapters/lcd_parallel_adapter.h"
#include "libraries/lcd/services/services.h"

int main()
{
	lcdPin_t lcdPinData[4] = { {.portRegister = GPIOA, .pinMask = 1 << 10},
			                   {.portRegister = GPIOA, .pinMask = 1 << 12},
							   {.portRegister = GPIOB, .pinMask = 1 << 6},
							   {.portRegister = GPIOB, .pinMask = 1 << 7} };

	lcdPin_t lcdPinReset = {.portRegister = GPIOB, .pinMask = 1 << 10};

	lcdPin_t lcdPinEnable = {.portRegister = GPIOB, .pinMask = 1 << 11};

	PORT_Init( PORTA );
	PORT_Init( PORTB );

	GPIO_InitOutputPin( GPIOA, 10, 0 );
	GPIO_InitOutputPin( GPIOA, 12, 0 );
	GPIO_InitOutputPin( GPIOB, 6, 0 );
	GPIO_InitOutputPin( GPIOB, 7, 0 );

	GPIO_InitOutputPin( GPIOB, 10, 0 );
	GPIO_InitOutputPin( GPIOB, 11, 0 );

	/* Initialize the adapter */
	lcdAdapter_t adapter = LCD_CreateParallelAdapter( lcdPinData, &lcdPinReset, &lcdPinEnable );

	/* Initialize the LCD */
	lcdHandle_t *lcd = LCD_Init(
		adapter, 16, 8, LCD_5x8_DOTS
	);

	/* Clear the lcd */
	LCD_Clear( lcd );

	/* Write Hello World! */
	LCD_WriteString( lcd, "Hello World!" );

	for ( ; ; )
	{

	}

	return 0;
}
