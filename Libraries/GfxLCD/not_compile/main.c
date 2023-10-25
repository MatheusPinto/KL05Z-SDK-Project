/* ###################################################################
**     Filename    : main.c
**     Project     : KL25_LCD_Sharp_400x240
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-01-07, 10:21, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "spi1_gen_driver.h"
#include "scsBit.h"
#include "BitIoLdd1.h"
#include "tftLcdData0.h"
#include "BitIoLdd2.h"
#include "spi1_k.h"
#include "tftLcdData1.h"
#include "BitIoLdd3.h"
#include "tftLcdData2.h"
#include "BitIoLdd4.h"
#include "tftLcdData3.h"
#include "BitIoLdd5.h"
#include "tftLcdData4.h"
#include "BitIoLdd6.h"
#include "tftLcdData5.h"
#include "BitIoLdd7.h"
#include "tftLcdData6.h"
#include "BitIoLdd8.h"
#include "tftLcdData7.h"
#include "BitIoLdd9.h"
#include "tftLcdCSX.h"
#include "BitIoLdd10.h"
#include "tftLcdRESX.h"
#include "BitIoLdd11.h"
#include "tftLcdDCX.h"
#include "BitIoLdd12.h"
#include "tftLcdWRX.h"
#include "BitIoLdd13.h"
#include "tftLcdRDX.h"
#include "BitIoLdd14.h"
#include "switch1.h"
#include "switch1.h"
#include "BitIoLdd16.h"
#include "adc0.h"
#include "AdcLdd1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "display.h"
#include "font_ArialBlack.h"
#include "font_CourierNew.h"
#include "font_TimesNewRomanItalic.h"
#include "ugui.h"
#include "tftlcd_ili9341.h"

void UserPixelSetFunction( UG_S16 x , UG_S16 y ,UG_COLOR c )
{
	tftLcd_DrawPixel(x, y, c);
}

void window_1_callback(UG_MESSAGE* msg) {
	if (msg->type == MSG_TYPE_OBJECT) {
		if (msg->id == OBJ_TYPE_BUTTON) {
			switch (msg->sub_id) {
			case BTN_ID_0: {
// . . .
				break;
			}
			case BTN_ID_1: {
// . . .
				break;
			}
			case BTN_ID_2: {
// . . .
				break;
			}
			}
		}
	}
}


/* Hardware a c c e l e r a t o r f o r UG DrawLine ( Plat form : STM32F4x9) */
UG_RESULT HW_DrawLine ( UG_S16 x0 , UG_S16 y0 , UG_S16 x1 , UG_S16 y1 , UG_COLOR color )
{
	int16_t steep = emb_Abs(y1 - y0) > emb_Abs(x1 - x0);

	if (steep)
	{
		emb_Swap(x0, y0);
		emb_Swap(x1, y1);
	}

	if (x0 > x1)
	{
		emb_Swap(x0, x1);
		emb_Swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = emb_Abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}

	for (; x0 <= x1; x0++)
	{
		if (steep)
		{
			tftLcd_DrawPixel(y0, x0, color);
		}
		else
		{
			tftLcd_DrawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
	return 0;
}

/* Hardware a c c e l e r a t o r f o r UG FillFrame ( Plat form : STM32F4x9) */
UG_RESULT HW_FillFrame ( UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c )
{
	tftLcd_FillRect(x1, y1, x2, y2, c);
	return 0;
}

#define MAX_OBJECTS 10

UG_GUI gui ; // Global GUI structure

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	UG_WINDOW window_1 ;
	UG_BUTTON button_1 ;
	UG_BUTTON button_2 ;
	UG_BUTTON button_3 ;
	UG_TEXTBOX textbox_1 ;
	UG_OBJECT obj_buff_wnd_1 [MAX_OBJECTS];
	UG_RESULT x;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  tftLcd_Init(); tftLcd_Begin();

  //HW_FillFrame(0, 0, 100, 200, C_BLUE);

  UG_Init(&gui , UserPixelSetFunction , 240 , 320);
  UG_DriverRegister( DRIVER_DRAW_LINE, ( void *)HW_DrawLine );
  UG_DriverRegister ( DRIVER_FILL_FRAME, ( void *) HW_FillFrame) ;
  UG_DriverEnable( DRIVER_DRAW_LINE );
  UG_DriverEnable( DRIVER_FILL_FRAME );
  UG_SelectGUI(&gui);

  //tftLcd_FillScreen(C_BLACK);
  //UG_FillFrame( 0 , 0 ,100 , 150 , C_BLACK);
  //UG_DrawCircle(100 , 100 , 30 , C_WHITE) ;
  //UG_DrawLine ( 0 , 0 , 120 , 70 , C_BLUE) ;

  /* Create the window */
  x = UG_WindowCreate( &window_1, obj_buff_wnd_1, MAX_OBJECTS, window_1_callback);
  /* Modify the window t i t l e */
  x = UG_WindowSetTitleText( &window_1, "uGUI com Kinetis" ) ;
  x = UG_WindowSetTitleTextFont( &window_1, &FONT_12X20) ;
  UG_WindowSetTitleColor ( &window_1 , C_DARK_GRAY );
  UG_WindowSetBackColor( &window_1 , dispDARK_GREY ) ;
  /* Create some but tons */
  x = UG_ButtonCreate( &window_1, &button_1, BTN_ID_0, 10, 10, 110, 60);
  x = UG_ButtonCreate( &window_1, &button_2, BTN_ID_1, 10, 80, 110, 130);
  x = UG_ButtonCreate( &window_1, &button_3, BTN_ID_2, 10, 150, 110, 200);
  /* Label the but tons */
  x = UG_ButtonSetFont( &window_1, BTN_ID_0, &FONT_12X20 );
  x = UG_ButtonSetText( &window_1, BTN_ID_0, "Button\nA" );
  x = UG_ButtonSetFont( &window_1, BTN_ID_1, &FONT_12X20 );
  x = UG_ButtonSetText( &window_1, BTN_ID_1, "Button\nB" );
  x = UG_ButtonSetFont( &window_1, BTN_ID_2, &FONT_12X20 );
  x = UG_ButtonSetText( &window_1, BTN_ID_2, "Button\nC" );
  /* Create a Textbox */
  x = UG_TextboxCreate( &window_1, &textbox_1, TXB_ID_0, 0, 220, 200, 280 ) ;
  x = UG_TextboxSetFont( &window_1, TXB_ID_0, &FONT_12X16);
  x = UG_TextboxSetText( &window_1, TXB_ID_0, "This is just a\n example!!!");
  x = UG_TextboxSetForeColor(&window_1, TXB_ID_0, C_BLACK);
  UG_TextboxSetBackColor( &window_1 , TXB_ID_0 , C_YELLOW ) ;
  x = UG_TextboxSetAlignment(&window_1, TXB_ID_0, ALIGN_CENTER);
  /* Finally , show the window */
  x = UG_WindowShow(&window_1);
  // . . .

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.4 [05.11]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
