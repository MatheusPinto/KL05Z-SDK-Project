/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "mcu_general_config.h"
#include "generic_drivers/lcd/lcd.h"
#include "libraries/delay/delay.h"
#include "libraries/emb_util/emb_util.h"
#include "drivers/port/port.h"
#include "drivers/gpio/gpio.h"
#include "drivers/tpm/tpm.h"
#include "drivers/adc/adc.h"


#define BOARD_TIMER_PORT PORTB
#define BOARD_TIMER_PIN 13

#define BOARD_KEY0_GPIO GPIOA
#define BOARD_KEY0_PORT PORTA
#define BOARD_KEY0_PIN 11
#define BOARD_KEY0_PIN_MASK (1 << BOARD_LCD_D4_PIN)

#define BOARD_KEY1_GPIO GPIOA
#define BOARD_KEY1_PORT PORTA
#define BOARD_KEY1_PIN 1
#define BOARD_KEY1_PIN_MASK (1 << BOARD_LCD_D4_PIN)

#define BOARD_LCD_D4_GPIO GPIOA
#define BOARD_LCD_D4_PORT PORTA
#define BOARD_LCD_D4_PIN 10
#define BOARD_LCD_D4_PIN_MASK (1 << BOARD_LCD_D4_PIN)

#define BOARD_LCD_D5_GPIO GPIOA
#define BOARD_LCD_D5_PORT PORTA
#define BOARD_LCD_D5_PIN 12
#define BOARD_LCD_D5_PIN_MASK (1 << BOARD_LCD_D5_PIN)

#define BOARD_LCD_D6_GPIO GPIOB
#define BOARD_LCD_D6_PORT PORTB
#define BOARD_LCD_D6_PIN 6
#define BOARD_LCD_D6_PIN_MASK (1 << BOARD_LCD_D6_PIN)

#define BOARD_LCD_D7_GPIO GPIOB
#define BOARD_LCD_D7_PORT PORTB
#define BOARD_LCD_D7_PIN 7
#define BOARD_LCD_D7_PIN_MASK (1 << BOARD_LCD_D7_PIN)

#define BOARD_LCD_RS_GPIO GPIOB
#define BOARD_LCD_RS_PORT PORTB
#define BOARD_LCD_RS_PIN 10
#define BOARD_LCD_RS_PIN_MASK (1 << BOARD_LCD_RS_PIN)

#define BOARD_LCD_EN_GPIO GPIOB
#define BOARD_LCD_EN_PORT PORTB
#define BOARD_LCD_EN_PIN 11
#define BOARD_LCD_EN_PIN_MASK (1 << BOARD_LCD_EN_PIN)

#define LCD_DELTA_CHAR 0x00

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


/*
 * Para rodar um exemplo, descomente sua macro correspondente e
 * deixe comentado as macros dos outros exemplos.
 *
 * */
#define EXEMPLO_1
//#define EXEMPLO_2
//#define EXEMPLO_3
//#define EXEMPLO_4


int main(void)
{
	lcdConfig_t* lcd0Config;
	lcdHandle_t lcd0Handle;

	PORT_Init(PORTA);
	PORT_Init(PORTB);

	GPIO_InitOutputPin(BOARD_LCD_D4_GPIO, BOARD_LCD_D4_PIN, 0);
	GPIO_InitOutputPin(BOARD_LCD_D5_GPIO, BOARD_LCD_D5_PIN, 0);
	GPIO_InitOutputPin(BOARD_LCD_D6_GPIO, BOARD_LCD_D6_PIN, 0);
	GPIO_InitOutputPin(BOARD_LCD_D7_GPIO, BOARD_LCD_D7_PIN, 0);
	GPIO_InitOutputPin(BOARD_LCD_RS_GPIO, BOARD_LCD_RS_PIN, 0);
	GPIO_InitOutputPin(BOARD_LCD_EN_GPIO, BOARD_LCD_EN_PIN, 0);

	Delay_Init();

	lcd0Config = LCD_CreateConfig();

	lcd0Config->lines = 2;
	lcd0Config->cols = 16;
	lcd0Config->charsize = LCD_5x8DOTS;
	lcd0Config->bus.data[0].portRegister = BOARD_LCD_D4_GPIO;
	lcd0Config->bus.data[0].pinMask = BOARD_LCD_D4_PIN_MASK;
	lcd0Config->bus.data[1].portRegister = BOARD_LCD_D5_GPIO;
	lcd0Config->bus.data[1].pinMask = BOARD_LCD_D5_PIN_MASK;
	lcd0Config->bus.data[2].portRegister = BOARD_LCD_D6_GPIO;
	lcd0Config->bus.data[2].pinMask = BOARD_LCD_D6_PIN_MASK;
	lcd0Config->bus.data[3].portRegister = BOARD_LCD_D7_GPIO;
	lcd0Config->bus.data[3].pinMask = BOARD_LCD_D7_PIN_MASK;
	lcd0Config->bus.rs.portRegister = BOARD_LCD_RS_GPIO;
	lcd0Config->bus.rs.pinMask = BOARD_LCD_RS_PIN_MASK;
	lcd0Config->bus.en.portRegister = BOARD_LCD_EN_GPIO;
	lcd0Config->bus.en.pinMask = BOARD_LCD_EN_PIN_MASK;

	lcd0Handle = LCD_Init(lcd0Config);

	//==========================EXEMPLO_1=================================
#ifdef EXEMPLO_1
	LCD_WriteString(lcd0Handle, "Ola mundo");

	for (;;) {
		LCD_ScrollDisplayLeft(lcd0Handle);
		Delay_Waitms(500);
	}
#endif
	//====================================================================

	//==========================EXEMPLO_2=================================
#ifdef EXEMPLO_2
	for(;;)
	{
		LCD_Write(lcd0Handle, 'A');
		Delay_Waitms(500);
	}
#endif
	//====================================================================

	//==========================EXEMPLO_3=================================
#ifdef EXEMPLO_3
	LCD_CreateChar(lcd0Handle, 0, myChar);

	LCD_SetCursor(lcd0Handle, 0, 0);

	for(;;)
	{
		LCD_Write(lcd0Handle, LCD_DELTA_CHAR);
		Delay_Waitms(500);
	}
#endif
	//====================================================================

	//==========================EXEMPLO_4=================================
#ifdef EXEMPLO_4
	LCD_CreateBigNumsChars(lcd0Handle);

	LCD_WriteBigNum(lcd0Handle, 0, 0);
	LCD_WriteBigNum(lcd0Handle, 2, 1);
	LCD_WriteBigNum(lcd0Handle, 5, 2);
	LCD_WriteBigNum(lcd0Handle, 8, 3);
	LCD_WriteBigNum(lcd0Handle, 11, 4);
	LCD_WriteBigNum(lcd0Handle, 14, 5);

	for(;;)
	{
		__asm("nop");
	}
#endif
	//====================================================================

	return 0;
}
