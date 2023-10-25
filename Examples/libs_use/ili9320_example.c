#include <MKL05Z4.h>
#include <system/system.h>

#include "mcu/drivers/port/port.h"
#include "mcu/drivers/gpio/gpio.h"
#include "libraries/delay/delay.h"
//#include "libraries/GfxLCD/tftlcd_ili9341.h"
#include "ili9320.h"

int main(void)
{
	//System_Init();

	PORT_Init(PORTA);
	PORT_Init(PORTB);

	GPIO_InitOutputPin( GPIOB, 8, 1 );
	GPIO_InitOutputPin( GPIOB, 9, 1 );
	GPIO_InitOutputPin( GPIOA, 8, 1 );
	GPIO_InitOutputPin( GPIOA, 0, 1 );

	GPIO_InitOutputPin( GPIOB, 10, 1 );
	GPIO_InitOutputPin( GPIOB, 11, 1 );
	GPIO_InitOutputPin( GPIOA, 11, 1 );
	GPIO_InitOutputPin( GPIOB, 5, 1 );
	GPIO_InitOutputPin( GPIOA, 10, 1 );
	GPIO_InitOutputPin( GPIOA, 12, 1 );
	GPIO_InitOutputPin( GPIOB, 6, 1 );
	GPIO_InitOutputPin( GPIOB, 7, 1 );

	Delay_Init();
	//tftLcd_Init();
	//tftLcd_Begin();

	//tftLcd_FillRect( 50, 50, 20, 30, 0xF800 );

	//tftLcd_FillScreen( 0xF800 );
/*
	ili9320_Init();
	for ( int i = 0; i < 30; ++i )
	{
		ili9320_DrawHLine(0xF800, 0, i, 50);
	}

	ili9320_DisplayOn();
	*/

	LCD_IO_WriteReg(0x3A);
	for ( ; ; )
	{

	}

	return 0;
}

