/***************************************************************************************
 * Module      : tftlcd_ili9341.c
 * Revision    : 1.0
 * Date        : 19/06/2016
 * Description : .
 * Comments    : None.
 * Author(s)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#include "tftlcd_ili9341.h"
#include <stddef.h>
#include <stdint.h>

/*MACROS*/
/*=======================================================================================*/


/*END: MACROS*/
/*=======================================================================================*/

/*PRIVATE VARIABLES*/
/*=======================================================================================*/

static tftHandler_t tft_handler =
{
	.width 			= tftWIDTH,
	.height 		= tftHEIGHT,
};

/*END: PRIVATE VARIABLES*/
/*=======================================================================================*/

/*PROTOTYPES - PRIVATE FUNCTIONS*/
/*=======================================================================================*/

void tftLcd_SetAddrWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void tftLcd_WriteCommand(uint8_t command, uint8_t *parameter, uint8_t paraNumber);
void tftLcd_Flood(uint16_t color, uint32_t len);

/*END: PROTOTYPES - PRIVATE FUNCTIONS*/
/*=======================================================================================*/


/*ABSTRACT TYPES*/
/*=======================================================================================*/


/*END: ABSTRACT TYPES*/
/*=======================================================================================*/


/*PUBLIC FUNCTIONS*/
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
void tftLcd_Init(void)
{
	// Set all control bits to idle state
	  tftLcd_ChipSelectIdle();
	  tftLcd_WriteIdle();
	  tftLcd_ReadIdle();
	  tftLcd_SetDataMode();
	  //The reset pin is set as 1 is Processor Expert

	  //tftLcd_setWriteDir(); // Set up LCD data port(s) for WRITE operations
}

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
void tftLcd_Begin(void)
{
    uint8_t tempParam[2];

    tftLcd_ChipSelectActive();

    // Reset any configuration or image in GRAM, and display off
    tempParam[0] = 0;
    tftLcd_WriteCommand(tftSOFTRESET_REG, &tempParam[0], 1);
    Delay_Waitms(3);
    tftLcd_WriteCommand(tftDISPLAYOFF_REG, &tempParam[0], 1);

    // Set the common voltage be 4,60 V and with high consumption and best quality
    tempParam[0] = tftGVDD_4_60_MASK;
    tftLcd_WriteCommand(tftPOWERCONTROL1_REG, &tempParam[0], 1);
    tempParam[0] = tftHP_CONSUPTION_MASK;
    tftLcd_WriteCommand(tftPOWERCONTROL2_REG, &tempParam[0], 1);

    // Set VCOMH = 3,750 V and VCOML = 1,450 V
    tempParam[0] = tftVCOMH_3_750_MASK;
    tempParam[1] = tftVCOML_1_450_MASK;
    tftLcd_WriteCommand(tftVCOM_CONTROL1_REG, tempParam, 2);

    // There is no VCOM offset
    //tempParam[0] = tftVCOM_OFFSET_0_MASK;
    tempParam[0] = 0xC0;
    tftLcd_WriteCommand(tftVCOM_CONTROL2_REG, &tempParam[0], 1);

    // Configure how the pixels will be stored in memory
    tempParam[0] = tftMADCTL_MY_MASK|tftMADCTL_BGR_MASK;
    tftLcd_WriteCommand(tftMEM_ACCESS_CONTROL_REG, &tempParam[0], 1);

    // Sets the pixel has 18 bits
    tempParam[0] = tftPIXEL_18BITS_MASK;
    tftLcd_WriteCommand(tftPIXELFORMAT_SET_REG, &tempParam[0], 1);

    // Sets 70 Hz frame rate
    tempParam[0] = 0x00; // MSB First
    tempParam[1] = (uint8_t)tftFRAME_RATE_70HZ_MASK;
    tftLcd_WriteCommand(tftFRAMERATE_CONTROL_REG, tempParam, 2);

    tempParam[0] = tftNORMAL_DISP_NO_LP_DETECT_MASK;
    tftLcd_WriteCommand(tftENTRYMODE_SET_REG, &tempParam[0], 1);

    tftLcd_WriteCommand(tftSLEEPOUT_REG, NULL, 0);
    Delay_Waitms(150);
    tftLcd_WriteCommand(tftDISPLAYON_REG, NULL, 0);
    Delay_Waitms(500);

    tftLcd_SetAddrWindow(0, 0, tft_handler.width-1, tft_handler.height-1);
}

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
uint8_t tftLcd_DrawPixel(int16_t xAxis, int16_t yAxis, uint16_t color)
{
	uint8_t color8[2];
	// Clip
	if((xAxis < 0) || (yAxis < 0) || (xAxis >= tft_handler.width) || (yAxis >= tft_handler.height)) return 0;

	//tftLcd_ChipSelectActive();

	tftLcd_SetAddrWindow(xAxis, yAxis, tft_handler.width-1, tft_handler.height-1);

	color8[0] = color >> 8; // MSB First
	color8[1] = color;
	tftLcd_WriteCommand(tftMEMORYWRITE_REG, color8, 2);

	//tftLcd_ChipSelectIdle();

	return 1;
}

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
void tftLcd_Reset(void)
{

}


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
void tftLcd_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color)
{

	int16_t  x2, y2;

  // Initial off-screen clipping
  if( (w <= 0) || (h <= 0) ||
	  (x >= tft_handler.width) || (y >= tft_handler.height) ||
      ((x2 = x+w-1) < 0 ) || ((y2 = y+h-1) <  0)) return;

  if(x < 0) { // Clip left
    w += x;
    x = 0;
  }
  if(y < 0) { // Clip top
    h += y;
    y = 0;
  }
  if(x2 >= tft_handler.width) { // Clip right
    x2 = tft_handler.width - 1;
    w  = x2 - x + 1;
  }
  if(y2 >= tft_handler.height) { // Clip bottom
    y2 = tft_handler.height - 1;
    h  = y2 - y + 1;
  }

  tftLcd_SetAddrWindow(x, y, x2, y2);
  tftLcd_Flood(color, (uint32_t)w * (uint32_t)h);
}

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
void tftLcd_FillScreen(uint16_t color)
{
  tftLcd_FillRect(0, 0, tft_handler.width, tft_handler.height, color);
}

void tftLcd_SetRotation(uint8_t x)
{
	   uint8_t t;

	   switch (x)
	   {
	   	   case 2:
	   		   t = tftMADCTL_MX_MASK | tftMADCTL_BGR_MASK;
	   		   break;
	   	   case 3:
	   		   t = tftMADCTL_MV_MASK | tftMADCTL_BGR_MASK;
	   		   Swap(tft_handler.width, tft_handler.height);
	   		   break;
	   	   case 0:
	   		   t = tftMADCTL_MY_MASK | tftMADCTL_BGR_MASK;
	   		   break;
	   	   case 1:
	   		   t = tftMADCTL_MX_MASK | tftMADCTL_MY_MASK | tftMADCTL_MV_MASK | tftMADCTL_BGR_MASK;
	   		Swap(tft_handler.width, tft_handler.height);
	   		   break;
	   }
	   tftLcd_WriteCommand(tftMEM_ACCESS_CONTROL_REG, &t, 1);
	   // For 9341, init default full-screen address window:
	   tftLcd_SetAddrWindow(0, 0, tft_handler.width - 1, tft_handler.height - 1);
}

/*END: PUBLIC FUNCTIONS*/
/*=======================================================================================*/


/*PRIVATE FUNCTIONS*/
/*=======================================================================================*/

/**********************************************************************
 * Function		:	tftLcd_SetAddrWindow
 *
 * Description	:   Sets the LCD address window.
 * 					Relevant to rect/screen fills and H/V lines.
 * 					Input coordinates are assumed pre-sorted (e.g. x2 >= x1).
 *
 * Inputs		:   x1 : the x1 position from the address window.
 * 					x2 : the x2 position from the address window.
 * 					y1 : the y1 position from the address window.
 * 					y2 : the y2 position from the address window.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void tftLcd_SetAddrWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  //tftLcd_ChipSelectActive();
  uint8_t temp[4];

  temp[0] = (uint8_t)(x1 >> 8);
  temp[1] = (uint8_t)x1;
  temp[2] = (uint8_t)(x2 >> 8);
  temp[3] = (uint8_t)x2;
  tftLcd_WriteCommand(tftCOLADDRSET_REG, temp, 4);

  temp[0] = (uint8_t)(y1 >> 8);
  temp[1] = (uint8_t)y1;
  temp[2] = (uint8_t)(y2 >> 8);
  temp[3] = (uint8_t)y2;
  tftLcd_WriteCommand(tftPAGEADDRSET_REG, temp, 4);

  //tftLcd_ChipSelectIdle();
}

/**********************************************************************
 * Function		:	tftLcd_WriteCommand
 *
 * Description	:   Write a set of parameters in a command register.
 *
 * Inputs		:   command    : the register address from command.
 * 					parameter  : the sequence of parameters from command.
 * 					paraNumber : the number of parameters.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void tftLcd_WriteCommand(uint8_t command, uint8_t *parameter, uint8_t paraNumber)
{
  tftLcd_SetCommandMode();
  tftLcd_Write8(command);
  tftLcd_SetDataMode();
  for(int i = 0; i < paraNumber; ++i)
  {
	  tftLcd_Write8(parameter[i]);
  }
}

// Fast block fill operation for fillScreen, fillRect, H/V line, etc.
// Requires setAddrWindow() has previously been called to set the fill
// bounds.  'len' is inclusive, MUST be >= 1.
void tftLcd_Flood(uint16_t color, uint32_t len)
{
  uint16_t blocks;
  uint8_t  i, color8[2];

  tftLcd_SetCommandMode();

  // Write first pixel normally, decrement counter by 1
  color8[0] = color >> 8,
  color8[1] = color;
  tftLcd_WriteCommand(0x2C, color8, 2);

  len--;
  blocks = (uint16_t)(len / 64); // 64 pixels/block
  if(color8[0] == color8[1]) { // (hi > lo?)
    // High and low bytes are identical.  Leave prior data
    // on the port(s) and just toggle the write strobe.
    while(blocks--) {
      i = 16; // 64 pixels/block / 4 pixels/pass
      do {
        tftLcd_WriteStrobe(); tftLcd_WriteStrobe(); tftLcd_WriteStrobe(); tftLcd_WriteStrobe(); // 2 bytes/pixel
        tftLcd_WriteStrobe(); tftLcd_WriteStrobe(); tftLcd_WriteStrobe(); tftLcd_WriteStrobe(); // x 4 pixels
      } while(--i);
    }
    // Fill any remaining pixels (1 to 64)
    for(i = (uint8_t)len & 63; i--; ) {
    	tftLcd_WriteStrobe();
    	tftLcd_WriteStrobe();
    }
  } else {
    while(blocks--) {
      i = 16; // 64 pixels/block / 4 pixels/pass
      do {
        tftLcd_Write8(color8[0]); tftLcd_Write8(color8[1]); tftLcd_Write8(color8[0]); tftLcd_Write8(color8[1]);
        tftLcd_Write8(color8[0]); tftLcd_Write8(color8[1]); tftLcd_Write8(color8[0]); tftLcd_Write8(color8[1]);
      } while(--i);
    }
    for(i = (uint8_t)len & 63; i--; ) {
    	tftLcd_Write8(color8[0]);
    	tftLcd_Write8(color8[1]);
    }
  }
}



/*END: PRIVATE FUNCTIONS*/
/*=======================================================================================*/

/* PROTOTYPES - ISRs
/*=======================================================================================*/


/* END: PROTOTYPES - ISRs
/*=======================================================================================*/

/***************************************************************************************
 * END: Module - tftlcd_ili9341.c
 ***************************************************************************************/
