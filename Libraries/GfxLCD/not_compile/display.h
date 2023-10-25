/***************************************************************************************
 * Module      : display.h
 * Revision    : 1.0
 * Date        : 16/06/2016
 *
 * Description : Definitions, prototypes and abstract types for LCD displays library.
 *
 * Comments    : This module contains an API with functions capable of generating designs
 * 				 such as circles, triangles, characters, etc. In theory, any LCD with any
 * 				 resolution and color scale can be used as target. Such displays should
 * 				 implement the basic functions of initialization, writing pixel and "flush",
 * 				 and associates them with "define" macros display_Init functions,
 * 				 display_DrawPixel and display_Flush, respectively.
 * 				 The display resolution should be associated with the dispWIDTH and
 * 				 dispHEIGHT macros.
 *
 * Author(s)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef DISPLAY_LIB_H_
#define DISPLAY_LIB_H_

#include "tftlcd_ili9341.h" /*Specific display driver header*/

/*ABSTRACT TYPES*/
/*=======================================================================================*/

typedef struct {
	const unsigned char *index;
	const unsigned char *unicode;
	const unsigned char *data;
	unsigned char version;
	unsigned char reserved;
	unsigned char index1_first;
	unsigned char index1_last;
	unsigned char index2_first;
	unsigned char index2_last;
	unsigned char bits_index;
	unsigned char bits_width;
	unsigned char bits_height;
	unsigned char bits_xoffset;
	unsigned char bits_yoffset;
	unsigned char bits_delta;
	unsigned char line_space;
	unsigned char cap_height;
} dispFont_t;

/*Data structure to handle the display driver internally*/
typedef struct dispHandler_struct_t
{
	uint8_t  driverInit;	/*Indicates if the driver was initialized*/
	int16_t  width;
	int16_t  height; // Display w/h as modified by current rotation
	int16_t  cursor_x;
	int16_t  cursor_y;
	uint16_t textColor;
	uint16_t textBGColor;
	uint8_t  textSize;
	uint8_t	 rotation;
	uint8_t  wrap; // If set, 'wrap' text at right edge of display
	const dispFont_t * font;
}dispHandler_t;

/*END: ABSTRACT TYPES*/
/*=======================================================================================*/

/*MACROS*/
/*=======================================================================================*/

/* some RGB color definitions 				 R,	  G,   B */
#define dispBLACK       	0x0000      /*   0,   0,   0 */
#define dispNAVY            0x000F      /*   0,   0,  16 */
#define dispDARK_GREEN      0x03E0      /*   0,  31,   0 */
#define dispDARK_CYAN       0x03EF      /*   0,  31,  16 */
#define dispMAROON          0x7800      /*  16,   0,   0 */
#define dispPURPLE          0x780F      /*  16,   0,  16 */
#define dispOLIVE           0x7BE0      /*  16,  16,   0 */
#define dispLIGHT_GREY      0xC618      /*  24,  48,  24 */
#define dispDARK_GREY       0x7BEF      /*  16,  16,  16 */
#define dispBLUE            0x001F      /*   0,   0,  31 */
#define dispGREEN           0x07E0      /*   0,  63,   0 */
#define dispCYAN            0x07FF      /*   0,  63,  31 */
#define dispRED             0xF800      /*  31,   0,   0 */
#define dispMAGENTA         0xF81F      /*  31,   0,  31 */
#define dispYELLOW          0xFFE0      /*  31,  63,   0 */
#define dispWHITE           0xFFFF      /*  31,  63,  31 */
#define dispORANGE          0xFD20      /*  31,  41,   0 */
#define dispGREEN_YELLOW    0xAFE5      /*  21,  63,   5 */
#define dispPINK            0xF81F		/*  31,   0,  31 */

/*Display resolution*/
#define dispWIDTH tftWIDTH
#define dispHEIGHT tftHEIGHT
#define dispFILL_RECT

/* Basic functions that must be implemented by the underlie LCD driver*/
#define display_Init(void) 					   {tftLcd_Init(); tftLcd_Begin();}
#define display_DrawPixel(xAxis, yAxis, color) tftLcd_DrawPixel(xAxis, yAxis, color)
#define display_Flush(void)
#define display_FillRect( x,  y,  w, h, color) tftLcd_FillRect( x,  y,  w, h, color)
#define display_hal_SetRotation(x) tftLcd_SetRotation(x)

/* Specific functions for touch displays*/
//#define display_IsTouching(void) tScreen_IsTouching()
//#define display_Pressure(void) tScreen_Pressure()
//#define display_ReadTouchY(void) tScreen_ReadTouchY()
//#define display_ReadTouchX(void) tScreen_ReadTouchX()

/*END: MACROS*/
/*=======================================================================================*/

/*ASCII table for 5x7 characters*/
extern const uint8_t fontTable5x7[];

/*PROTOTYPES - PUBLIC FUNCTIONS*/
/*=======================================================================================*/

void display_SetFont(const dispFont_t *font);

/**********************************************************************
 * Function		:	display_DrawCircle
 *
 * Description	:   Draw a circle in display.
 *
 * Inputs		:   x0   : the x position from circle center.
 * 					y0	 : the y position from circle center.
 * 					rad	 : the radius from circle.
 * 					color: border color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_DrawCircle(int16_t x0, int16_t y0, int16_t rad, uint16_t color);

/**********************************************************************
 * Function		:	display_FillCircle
 *
 * Description	:   Fill a circle in display with a color.
 *
 * Inputs		:   x0   : the x position from circle center.
 * 					y0	 : the y position from circle center.
 * 					rad	 : the radius from circle.
 * 					color: the fill color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_FillCircle(int16_t x0, int16_t y0, int16_t rad, uint16_t color);

/**********************************************************************
 * Function		:	display_DrawLineCircle
 *
 * Description	:   Draw a circle in display with a certain thickness contour.
 *
 * Inputs		:   x0   : the x position from circle center.
 * 					y0	 : the y position from circle center.
 * 					rad	 : the radius from circle.
 * 					color: border color.
 * 					line : thickness of contour.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_DrawLineCircle(int16_t x0, int16_t y0, int16_t rad, uint16_t color, uint8_t line);

/**********************************************************************
 * Function		:	display_DrawCircleHelper
 *
 * Description	:   .
 *
 * Inputs		:   x0   	   : the x position from circle center.
 * 					y0	 	   : the y position from circle center.
 * 					rad	 	   : the radius from circle.
 * 					connername :
 * 					color	   : border color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_DrawCircleHelper( int16_t x0, int16_t y0, int16_t rad, uint8_t cornername, uint16_t color);

/**********************************************************************
 * Function		:	display_FillCircleHelper
 *
 * Description	:   .
 *
 * Inputs		:   x0   	   : the x position from circle center.
 * 					y0	 	   : the y position from circle center.
 * 					rad	 	   : the radius from circle.
 * 					connername :
 * 					delta	   :
 * 					color	   :
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_FillCircleHelper(int16_t x0, int16_t y0, int16_t rad, uint8_t cornername, int16_t delta, uint16_t color);

/**********************************************************************
 * Function		:	display_DrawLine
 *
 * Description	:   Draw a line in display, using the Bresenham algorithm.
 *
 * Inputs		:   x0   : the x position from 0 point.
 * 					y0	 : the y position from 0 point.
 * 					x1   : the x position from 1 point.
 * 					y1	 : the y position from 1 point.
 * 					color: line color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

/**********************************************************************
 * Function		:	display_DrawFastVLine
 *
 * Description	:   Draw a vertical line in display in a fast way.
 *
 * Inputs		:   x    : the x position from start point.
 * 					y	 : the y position from start point.
 * 					h	 : height.
 * 					color: line color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

/**********************************************************************
 * Function		:	display_DrawFastHLine
 *
 * Description	:   Draw a horizontal line in display in a fast way.
 *
 * Inputs		:   x    : the x position from start point.
 * 					y	 : the y position from start point.
 * 					w	 : width.
 * 					color: line color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

/**********************************************************************
 * Function		:	display_DrawRect
 *
 * Description	:   Draw a rectangle in display.
 *
 * Inputs		:   x    : the x position from center.
 * 					y	 : the y position from center.
 * 					w    : width.
 * 					h	 : height.
 * 					color: border color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

#ifndef dispFILL_RECT
/**********************************************************************
 * Function		:	display_FillRect
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
void display_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
#endif //dispFILL_RECT

/**********************************************************************
 * Function		:	display_DrawRoundRect
 *
 * Description	:   Draw a rectangle with rounded edges.
 *
 * Inputs		:   x    : the x position from center.
 * 					y	 : the y position from center.
 * 					w	 : width.
 * 					h	 : height.
 * 					color: border color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_DrawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t rad, uint16_t color);

/**********************************************************************
 * Function		:	display_FillRoundRect
 *
 * Description	:   Fill with a specified color, a rectangle with rounded edges.
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
void display_FillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t rad, uint16_t color) ;

/**********************************************************************
 * Function		:	display_DrawTriangle
 *
 * Description	:   Draw a triangle in display..
 *
 * Inputs		:   x0   : the x position from vertex 0.
 * 					y0	 : the y position from vertex 0.
 * 					x1   : the x position from vertex 1.
 * 					y1	 : the y position from vertex 1.
 * 					x2   : the x position from vertex 2.
 * 					y2	 : the y position from vertex 2.
 * 					color: border color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_DrawTriangle(int16_t x0, int16_t y0,	int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

/**********************************************************************
 * Function		:	display_FillTriangle
 *
 * Description	:   Fill a triangle in display with a specified color.
 *
 * Inputs		:   x0   : the x position from vertex 0.
 * 					y0	 : the y position from vertex 0.
 * 					x1   : the x position from vertex 1.
 * 					y1	 : the y position from vertex 1.
 * 					x2   : the x position from vertex 2.
 * 					y2	 : the y position from vertex 2.
 * 					color: fill color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_FillTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

/**********************************************************************
 * Function		:	display_FillTriangle
 *
 * Description	:   Fill the entire display screen with specified color.
 *
 * Inputs		:  	color: fill color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_FillScreen(uint16_t color);

/**********************************************************************
 * Function		:	display_DrawBitmap
 *
 * Description	:   Draw a 1-bit color bitmap in a x, y position provided
 * 					by a buffer with the bitmap (the buffer should be in the program memory!).
 *
 * Inputs		:  	x	  : the x position from up-left vertex from bitmap.
 * 					y	  : the y position from up-left vertex from bitmap.
 * 					bitmap: pointer to the buffer that stores de bitmap.
 * 					w	  : width.
 * 					h	  : height.
 * 					color : foreground color.
 * 					bg    : background color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_DrawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);

/**********************************************************************
 * Function		:	display_DrawXBitmap
 *
 * Description	:   Draw xBitMap files (*.xbm), exported from GIMP.
 *
 * Inputs		:  	x	  : the x position from up-letf vertex from bitmap.
 * 					y	  : the y position from up-letf vertex from bitmap.
 * 					bitmap: pointer to the buffer that stores de bitmap.
 * 					w	  : width.
 * 					h	  : height.
 * 					color : foreground color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	Export from GIMP to *.xbm, rename from *.xbm to *.c
 * 					and open the IDE/Editor. Arrays in C can be used
 * 					directly with this function.
 * ********************************************************************/
void display_DrawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color );

/**********************************************************************
 * Function		:	display_DrawChar
 *
 * Description	:   Draw a char with 5x7 resolution.
 *
 * Inputs		:  	x: the x position.
 * 					y: the y position.
 * 					color: character color.
 * 					bg: background color.
 * 					size: character increase
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_DrawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);

/**********************************************************************
 * Function		:	display_Print
 *
 * Description	:   Draws a text with a resolution of 5x7 characters.
 * 					The position of the text depends on where the cursor is.
 * 					The position can be observed with display_GetTextCursor function
 * 					and can be adjusted with display_SetTextCursor function.
 * 					The default color of the character is black and white background,
 * 					but can be modified in display_SetTextColor functions.
 * 					The size of the characters by default is 1, but can be modified
 * 					in display_SetTextSize function.
 *
 * Inputs		:  	text: string that contains the text.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_Print( unsigned char const *text);

/**********************************************************************
 * Function		:	display_SetTextCursor
 *
 * Description	:   Sets the current position cursor from text.
 *
 * Inputs		:  	x, y: the x and y position.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_SetTextCursor(int16_t x, int16_t y);

/**********************************************************************
 * Function		:	display_GetTextCursor
 *
 * Description	:   Gets the current position cursor from text.
 *
 * Inputs		:  	x, y: the variable that will stores x and y position.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_GetTextCursor(int16_t *x, int16_t *y);

/**********************************************************************
 * Function		:	display_SetTextSize
 *
 * Description	:   Sets the text size.
 *
 * Inputs		:  	s: size.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_SetTextSize(uint8_t s);

/**********************************************************************
 * Function		:	display_SetTextColor
 *
 * Description	:   Sets the text color.
 *
 * Inputs		:  	color: text color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_SetTextColor(uint16_t color);

/**********************************************************************
 * Function		:	display_SetTextBGColor
 *
 * Description	:   Sets the text background color.
 *
 * Inputs		:  	color: background color.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_SetTextBGColor(uint16_t color);

uint16_t display_GetTextColor(void);

/**********************************************************************
 * Function		:	display_SetTextWrap
 *
 * Description	:
 *
 * Inputs		:
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_SetTextWrap(uint8_t w);

/**********************************************************************
 * Function		:	display_SetRotation
 *
 * Description	:
 *
 * Inputs		:
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_SetRotation(uint8_t x);

/**********************************************************************
 * Function		:	display_GetRotation
 *
 * Description	:
 *
 * Inputs		:
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
uint8_t display_GetRotation(void);

/**********************************************************************
 * Function		:	display_Width
 *
 * Description	:
 *
 * Inputs		:
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
int16_t display_Width(void);

/**********************************************************************
 * Function		:	display_Heigth
 *
 * Description	:
 *
 * Inputs		:
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
int16_t display_Height(void);

/**********************************************************************
 * Function		:	display_Invert
 *
 * Description	:
 *
 * Inputs		:
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void display_Invert(uint8_t i);

/*END: PROTOTYPES - PUBLIC FUNCTIONS*/
/*=======================================================================================*/

#endif /* DISPLAY_LIB_H_ */

/***************************************************************************************
 * END: Module - display.h
 ***************************************************************************************/
