/***************************************************************************************
 * Module      : tftlcd_ili9341.h
 * Revision    : 1.0
 * Date        : 19/06/2016
 * Description : .
 * Comments    : None.
 * Author(s)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef TFTLCD_ILI9341_H_
#define TFTLCD_ILI9341_H_

#include <MKL05Z4.h>
#include "libraries/delay/delay.h"
#include "libraries/util/swap.h"

/*MACROS*/
/*=======================================================================================*/

// ILI9341 registers address
#define tftSOFTRESET_REG	      	0x01
#define tftSLEEPIN_REG            	0x10
#define tftSLEEPOUT_REG           	0x11
#define tftNORMALDISP_REG         	0x13
#define tftINVERTOFF_REG          	0x20
#define tftINVERTON_REG           	0x21
#define tftGAMMASET_REG           	0x26
#define tftDISPLAYOFF_REG         	0x28
#define tftDISPLAYON_REG          	0x29
#define tftCOLADDRSET_REG         	0x2A
#define tftPAGEADDRSET_REG        	0x2B
#define tftMEMORYWRITE_REG        	0x2C
#define tftMEM_ACCESS_CONTROL_REG   0x36
#define tftMADCTL_REG  		   	  	0x36
#define tftPIXELFORMAT_SET_REG      0x3A
#define tftFRAMERATE_CONTROL_REG    0xB1
#define tftDISPLAYFUNC_REG        	0xB6
#define tftENTRYMODE_SET_REG        0xB7
#define tftPOWERCONTROL1_REG      	0xC0
#define tftPOWERCONTROL2_REG      	0xC1
#define tftVCOM_CONTROL1_REG       	0xC5
#define tftVCOM_CONTROL2_REG       	0xC7

// Masks to set/clear ILI9341 registers data
#define tftMADCTL_MY_MASK  					0x80
#define tftMADCTL_MX_MASK  					0x40
#define tftMADCTL_MV_MASK  					0x20
#define tftMADCTL_ML_MASK  					0x10
#define tftMADCTL_RGB_MASK 					0x00
#define tftMADCTL_BGR_MASK 					0x08
#define tftMADCTL_MH_MASK  					0x04
#define tftGVDD_4_60_MASK  					0x23
#define tftHP_CONSUPTION_MASK 				0x10
#define tftVCOMH_3_750_MASK 				0x2B
#define tftVCOML_1_450_MASK 				0x2B
#define tftVCOM_OFFSET_0_MASK 				0x00
#define tftPIXEL_18BITS_MASK 				0x55
#define tftFRAME_RATE_70HZ_MASK 			0x001B
#define tftNORMAL_DISP_NO_LP_DETECT_MASK 	0x07

// the lcd dimension
#define tftWIDTH  240
#define tftHEIGHT 320


#define tftLcdRDX_ClrVal() GPIOB->PCOR = 1 << 8
#define tftLcdRDX_SetVal() GPIOB->PSOR = 1 << 8
#define tftLcdWRX_ClrVal() GPIOB->PCOR = 1 << 9
#define tftLcdWRX_SetVal() GPIOB->PSOR = 1 << 9
#define tftLcdDCX_ClrVal() GPIOA->PCOR = 1 << 8
#define tftLcdDCX_SetVal() GPIOA->PSOR = 1 << 8
#define tftLcdCSX_ClrVal() GPIOA->PCOR = 1 << 0
#define tftLcdCSX_SetVal() GPIOA->PSOR = 1 << 0

#define tftLcdData0_PutVal(x) {if ( x ) GPIOB->PSOR = 1 << 10; else GPIOB->PCOR = 1 << 10;}
#define tftLcdData1_PutVal(x)  {if ( x ) GPIOB->PSOR = 1 << 11; else GPIOB->PCOR = 1 << 11;}
#define tftLcdData2_PutVal(x)  {if ( x ) GPIOA->PSOR = 1 << 11; else GPIOA->PCOR = 1 << 11;}
#define tftLcdData3_PutVal(x)  {if ( x ) GPIOB->PSOR = 1 << 5; else GPIOB->PCOR = 1 << 5;}
#define tftLcdData4_PutVal(x)  {if ( x ) GPIOA->PSOR = 1 << 10; else GPIOA->PCOR = 1 << 10;}
#define tftLcdData5_PutVal(x)  {if ( x ) GPIOA->PSOR = 1 << 12; else GPIOA->PCOR = 1 << 12;}
#define tftLcdData6_PutVal(x)  {if ( x ) GPIOB->PSOR = 1 << 6; else GPIOB->PCOR = 1 << 6;}
#define tftLcdData7_PutVal(x)  {if ( x ) GPIOB->PSOR = 1 << 7; else GPIOB->PCOR = 1 << 7;}


/*ABSTRACT TYPES*/
/*=======================================================================================*/

/*Data structure to handle the tft lcd driver internally*/
typedef struct tftHandler_struct_t
{
	//uint8_t  driverInit;	/*Indicates if the driver was initialized*/
	int16_t  width;
	int16_t  height; // Display w/h as modified by current rotation
	//int16_t  cursor_x;
	//int16_t  cursor_y;
	//uint16_t textColor;
	//uint16_t textBGColor;
	//uint8_t  textSize;
	//uint8_t	 rotation;
	//uint8_t  wrap; // If set, 'wrap' text at right edge of display
}tftHandler_t;

/*END: ABSTRACT TYPES*/
/*=======================================================================================*/

#define tftLcd_DisplayOff(void) \
	{\
		tftLcd_SetCommandMode();\
		tftLcd_Write8(tftDISPLAYOFF_REG);\
		tftLcd_SetDataMode();\
		tftLcd_Write8(0x00);\
  	}

#define tftLcd_DisplayOn(void) \
	{\
		tftLcd_SetCommandMode();\
		tftLcd_Write8(tftDISPLAYON_REG);\
		Delay_Waitms(500);\
  	}

// LCD control lines:
// Control signals are ACTIVE LOW (idle is HIGH)
// Command/Data: LOW = command, HIGH = data
// These are single-instruction operations and always inline

/**********************************************************************
 * Function		:	tftLcd_ReadActive
 *
 * Description	:   Set the LCD driver to Read the Data/Command in Data Lines.
 *
 * Inputs		:   None.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
#define tftLcd_ReadActive(void)			tftLcdRDX_ClrVal();

/**********************************************************************
 * Function		:	tftLcd_ReadIdle
 *
 * Description	:   The Read line is set to a IDLE mode.
 *
 * Inputs		:   None.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
#define tftLcd_ReadIdle(void)			tftLcdRDX_SetVal();
#define tftLcd_WriteActive(void)		tftLcdWRX_ClrVal();
#define tftLcd_WriteIdle(void)			tftLcdWRX_SetVal();
#define tftLcd_SetCommandMode(void)		tftLcdDCX_ClrVal();
#define tftLcd_SetDataMode(void)		tftLcdDCX_SetVal();
#define tftLcd_ChipSelectActive(void)	tftLcdCSX_ClrVal();
#define tftLcd_ChipSelectIdle(void)		tftLcdCSX_SetVal();

// Data write strobe, ~2 instructions and always inline
#define tftLcd_WriteStrobe(void) { tftLcd_WriteActive(); tftLcd_WriteIdle(); }

#define tftLcd_setWriteDir(void) \
{\
	tftLcdData0_SetDir(TRUE);\
	tftLcdData1_SetDir(TRUE);\
	tftLcdData2_SetDir(TRUE);\
	tftLcdData3_SetDir(TRUE);\
	tftLcdData4_SetDir(TRUE);\
	tftLcdData5_SetDir(TRUE);\
	tftLcdData6_SetDir(TRUE);\
	tftLcdData7_SetDir(TRUE);\
}

#define tftLcd_setReadDir(void) \
{\
	tftLcdData0_SetDir(FALSE);\
	tftLcdData1_SetDir(FALSE);\
	tftLcdData2_SetDir(FALSE);\
	tftLcdData3_SetDir(FALSE);\
	tftLcdData4_SetDir(FALSE);\
	tftLcdData5_SetDir(FALSE);\
	tftLcdData6_SetDir(FALSE);\
	tftLcdData7_SetDir(FALSE);\
}

#define tftLcd_Write8(x) \
{\
	tftLcdData0_PutVal((x & 0x01));\
	tftLcdData1_PutVal((x & 0x02));\
	tftLcdData2_PutVal((x & 0x04));\
	tftLcdData3_PutVal((x & 0x08));\
	tftLcdData4_PutVal((x & 0x10));\
	tftLcdData5_PutVal((x & 0x20));\
	tftLcdData6_PutVal((x & 0x40));\
	tftLcdData7_PutVal((x & 0x80));\
	Delay_Waitus(70);\
	tftLcd_WriteStrobe(); /*Generates the falling edge for LCD latches the write data*/\
}

#define tftLcd_Read8(result) \
{\
	tftLcd_ReadActive();\
	delayer_Waitms(7);\
	result = (tftLcdData0_GetVal()) 		|\
			(tftLcdData1_GetVal() << 1) |\
			(tftLcdData2_GetVal() << 2) |\
			(tftLcdData3_GetVal() << 3) |\
			(tftLcdData4_GetVal() << 4) |\
			(tftLcdData5_GetVal() << 5) |\
			(tftLcdData6_GetVal() << 6) |\
			(tftLcdData7_GetVal() << 7) |\
	tftLcd_ReadIdle();\
}


/*END: MACROS*/
/*=======================================================================================*/

/*ABSTRACT TYPES*/
/*=======================================================================================*/


/*END: ABSTRACT TYPES*/
/*=======================================================================================*/


/*PROTOTYPES - PUBLIC FUNCTIONS*/
/*=======================================================================================*/

/**********************************************************************
 * Function		:	tftLcd_Init
 *
 * Description	:   Initialize the ili9341 controller driver.
 *
 * Inputs		:   None.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void tftLcd_Init(void);

/**********************************************************************
 * Function		:	tftLcd_Begin
 *
 * Description	:   Set the parameters for driver began to draw.
 *
 * Inputs		:   None.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void tftLcd_Begin(void);

/**********************************************************************
 * Function		:	tftLcd_DrawPixel
 *
 * Description	:   Draw a pixel in touch screen display.
 *
 * Inputs		:   xAxis : the x position from pixel.
 * 					yAxis : the y position from pixel.
 * 					color : pixel color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
uint8_t tftLcd_DrawPixel(int16_t xAxis, int16_t yAxis, uint16_t color);

/**********************************************************************
 * Function		:	tftLcd_Reset
 *
 * Description	:   Reset the display driver.
 *
 * Inputs		:   None.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void tftLcd_Reset(void);

/**********************************************************************
 * Function		:	tftLcd_FillRect
 *
 * Description	:   Fill a rectangle in display with a specified color.
 *
 * Inputs		:   x    : the x position from center.
 * 					y	 : the y position from center.
 * 					w	 : width.
 * 					h	 : height.
 * 					color: fill color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void tftLcd_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);

/**********************************************************************
 * Function		:	tftLcd_FillScreen
 *
 * Description	:   Fill the entire display screen with specified color.
 *
 * Inputs		:  	color: fill color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void tftLcd_FillScreen(uint16_t color);


void tftLcd_SetRotation(uint8_t x);

/*END: PROTOTYPES - PUBLIC FUNCTIONS*/
/*=======================================================================================*/

/* PROTOTYPES - ISRs
/*=======================================================================================*/


/* END: PROTOTYPES - ISRs
/*=======================================================================================*/

#endif /* TFTLCD_ILI9341_H_ */

/***************************************************************************************
 * END: Module - tftlcd_ili9341.h
 ***************************************************************************************/



