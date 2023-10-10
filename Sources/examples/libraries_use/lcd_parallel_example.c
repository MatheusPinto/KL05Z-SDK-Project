#include "mcu/drivers/port/port.h"
#include "mcu/drivers/gpio/gpio.h"
#include "libraries/delay/delay.h"
#include "libraries/lcd/adapters/lcd_parallel_adapter.h"
#include "libraries/lcd/services/services.h"

int main()
{
	// D4 = lcdPinData[0], D5 = lcdPinData[1], D6 = lcdPinData[2], D7 = lcdPinData[3]
	lcdPin_t lcdPinData[4] = { {.portRegister = GPIOA, .pinMask = 1 << 10},
			                   {.portRegister = GPIOA, .pinMask = 1 << 12},
							   {.portRegister = GPIOB, .pinMask = 1 << 6},
							   {.portRegister = GPIOB, .pinMask = 1 << 7} };


	lcdPin_t lcdPinRS = {.portRegister = GPIOB, .pinMask = 1 << 10};

	lcdPin_t lcdPinEnable = {.portRegister = GPIOB, .pinMask = 1 << 11};

	PORT_Init( PORTA );
	PORT_Init( PORTB );

	GPIO_InitOutputPin( GPIOA, 10, 0 );
	GPIO_InitOutputPin( GPIOA, 12, 0 );
	GPIO_InitOutputPin( GPIOB, 6, 0 );
	GPIO_InitOutputPin( GPIOB, 7, 0 );

	GPIO_InitOutputPin( GPIOB, 10, 0 );
	GPIO_InitOutputPin( GPIOB, 11, 0 );

	Delay_Init();

	/* Initialize the adapter */
	lcdAdapter_t adapter = LCD_CreateParallelAdapter( lcdPinData, &lcdPinRS, &lcdPinEnable );

	/* Initialize the LCD */
	lcdHandle_t *lcd = LCD_Init(
		adapter, 16, 8, LCD_5x8_DOTS
	);

	/* Clear the lcd */
	LCD_Clear( lcd );

	/* Write Hello World! */
	LCD_WriteString( lcd, "Hello World!" );
	Delay_Waitms(1000);

	for ( ; ; )
	{
		LCD_ScrollDisplayLeft( lcd );
		Delay_Waitms(500);
	}

	return 0;
}
