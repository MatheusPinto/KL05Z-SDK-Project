#include "mcu/drivers/port/port.h"
#include "mcu/drivers/gpio/gpio.h"
#include "libraries/delay/delay.h"
#include "libraries/lcd/adapters/lcd_parallel_adapter.h"
#include "libraries/lcd/adapters/lcd_i2c_adapter.h"
#include "libraries/lcd/lcd.h"

const uint8_t myChar[] =
{
		0b00100,
		0b00100,
		0b01010,
		0b01010,
		0b10001,
		0b11111,
		0b00000,
		0b00000,
		0b00000
};

#define LCD_DELTA_CHAR 0x00

/*
 * Para rodar um exemplo, descomente sua macro correspondente e
 * deixe comentado as macros dos outros exemplos.
 *
 * To run an example, uncomment its corresponding macro and leave
 * the macros for the other examples commented.
 *
 * */
#define EXEMPLO_1
//#define EXEMPLO_2
//#define EXEMPLO_3
//#define EXEMPLO_4

/*
 * @brief   Application entry point.
 */
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

	/* Initialize a parallel adapter */
	lcdAdapter_t adapter = LCD_CreateParallelAdapter( lcdPinData, &lcdPinRS, &lcdPinEnable );

	/* Initialize a I2C adapter */
	//lcdAdapter_t adapter = LCD_CreateI2CAdapter( I2C0, 0x3f );

	/* Initialize the LCD */
	lcdHandle_t lcd = LCD_Init(
		adapter, 16, 8, LCD_5x8_DOTS
	);

	/* Clear the lcd */
	LCD_Clear( lcd );

    //==========================EXEMPLO_1=================================
#ifdef EXEMPLO_1
    LCD_WriteString(lcd, "Hello World!");
    Delay_Waitms(1000);

    for(;;)
    {
    	LCD_ScrollDisplayLeft(lcd);
    	Delay_Waitms(500);
    }
#endif
    //====================================================================


    //==========================EXEMPLO_2=================================
#ifdef EXEMPLO_2
    for(;;)
    {
    	LCD_Write(lcd, 'A');
    	Delay_Waitms(500);
    }
#endif
    //====================================================================

    //==========================EXEMPLO_3=================================
#ifdef EXEMPLO_3
    LCD_CreateChar(lcd, 0, myChar);

    LCD_SetCursor(lcd, 0, 0);

    for(;;)
    {
    	LCD_Write(lcd, LCD_DELTA_CHAR);
    	Delay_Waitms(500);
    }
#endif
    //====================================================================

    //==========================EXEMPLO_4=================================
#ifdef EXEMPLO_4
    LCD_CreateBigNumsChars(lcd);

    LCD_WriteBigNum(lcd, 0, 0);
    LCD_WriteBigNum(lcd, 2, 1);
    LCD_WriteBigNum(lcd, 5, 2);
    LCD_WriteBigNum(lcd, 8, 3);
    LCD_WriteBigNum(lcd, 11, 4);
    LCD_WriteBigNum(lcd, 14, 5);

    for(;;)
    {
    	__asm("nop");
    }
#endif
    //====================================================================


	return 0;
}
