/***************************************************************************************
 * Module      : display.c
 * Revision    : 1.0
 * Date        : 16/06/2016
 *
 * Description : Library for drawings generation in LCD displays, colored or monochrome.
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

#include "display.h"
#include "emb_util.h"

/*PRIVATE VARIABLES*/
/*=======================================================================================*/

static dispHandler_t display_handler =
{
	.driverInit 	= pdFALSE,
	.width 			= dispWIDTH,
	.height 		= dispHEIGHT,
	.cursor_x 		= 0,
	.cursor_y 		= 0,
	.textColor 		= dispWHITE,
	.textBGColor 	= dispWHITE,
	.textSize 		= 1,
	.rotation 		= 0,
	.wrap			= 1,
	.font			= NULL
};

/*END: PRIVATE VARIABLES*/
/*=======================================================================================*/

void display_DrawFontChar(unsigned int c);

static uint32_t fetchbit(const uint8_t *p, uint32_t index);

static uint32_t fetchbits_unsigned(const uint8_t *p, uint32_t index, uint32_t required);

static uint32_t fetchbits_signed(const uint8_t *p, uint32_t index, uint32_t required);

static void drawFontBits(uint32_t bits, uint32_t numbits, uint32_t x, uint32_t y, uint32_t repeat);

static void drawFontBits(uint32_t bits, uint32_t numbits, uint32_t x, uint32_t y, uint32_t repeat)
{
	// TODO: replace this *slow* code with something fast...
	//printf("      %d bits at %d,%d: %X\n", numbits, x, y, bits);
	if (bits == 0) return;
	do {
		uint32_t x1 = x;
		uint32_t n = numbits;
		do {
			n--;
			if (bits & (1 << n)) {
				display_DrawPixel(x1, y, display_handler.textColor);
				//printf("        pixel at %d,%d\n", x1, y);
			}
			x1++;
		} while (n > 0);
		y++;
		repeat--;
	} while (repeat);

}

static uint32_t fetchbit(const uint8_t *p, uint32_t index)
{
	if (p[index >> 3] & (1 << (7 - (index & 7)))) return 1;
	return 0;
}

static uint32_t fetchbits_unsigned(const uint8_t *p, uint32_t index, uint32_t required)
{
	uint32_t val = 0;
	do {
		uint8_t b = p[index >> 3];
		uint32_t avail = 8 - (index & 7);
		if (avail <= required) {
			val <<= avail;
			val |= b & ((1 << avail) - 1);
			index += avail;
			required -= avail;
		} else {
			b >>= avail - required;
			val <<= required;
			val |= b & ((1 << required) - 1);
			break;
		}
	} while (required);
	return val;
}

static uint32_t fetchbits_signed(const uint8_t *p, uint32_t index, uint32_t required)
{
	uint32_t val = fetchbits_unsigned(p, index, required);
	if (val & (1 << (required - 1))) {
		return (int32_t)val - (1 << required);
	}
	return (int32_t)val;
}

uint8_t display_WriteChar(uint8_t c)
{
	if (display_handler.font) {
		if (c == '\n') {
			//cursor_y += ??
			display_handler.cursor_x = 0;
		} else {
			display_DrawFontChar(c);
		}
	} else {
		if (c == '\n') {
			display_handler.cursor_y += display_handler.textSize*8;
			display_handler.cursor_x  = 0;
		} else if (c == '\r') {
			// skip em
		} else {
			display_DrawChar(display_handler.cursor_x, display_handler.cursor_y, c, display_handler.textColor, display_handler.textBGColor, display_handler.textSize);
			display_handler.cursor_x += display_handler.textSize*6;
			if (display_handler.wrap && (display_handler.cursor_x > (display_handler.width - display_handler.textSize*6))) {
				display_handler.cursor_y += display_handler.textSize*8;
				display_handler.cursor_x = 0;
			}
		}
	}
	return 1;
}

/*PUBLIC FUNCTIONS*/
/*=======================================================================================*/

void display_SetFont(const dispFont_t *font)
{
	display_handler.font = font;
}

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
void display_DrawCircle(int16_t x0, int16_t y0, int16_t rad, uint16_t color)
{
  int16_t f = 1 - rad;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * rad;
  int16_t x = 0;
  int16_t y = rad;

  display_DrawPixel(x0  , y0+rad, color);
  display_DrawPixel(x0  , y0-rad, color);
  display_DrawPixel(x0+rad, y0  , color);
  display_DrawPixel(x0-rad, y0  , color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    display_DrawPixel(x0 + x, y0 + y, color);
    display_DrawPixel(x0 - x, y0 + y, color);
    display_DrawPixel(x0 + x, y0 - y, color);
    display_DrawPixel(x0 - x, y0 - y, color);
    display_DrawPixel(x0 + y, y0 + x, color);
    display_DrawPixel(x0 - y, y0 + x, color);
    display_DrawPixel(x0 + y, y0 - x, color);
    display_DrawPixel(x0 - y, y0 - x, color);
  }
}

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
void display_FillCircle( int16_t x0, int16_t y0, int16_t rad, uint16_t color)
{
  display_DrawFastVLine(x0, y0-rad, 2*rad+1, color);
  display_FillCircleHelper(x0, y0, rad, 3, 0, color);
}

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
void display_DrawLineCircle(int16_t x0, int16_t y0, int16_t rad, uint16_t color, uint8_t line)
{
	for(int i = 0; i < line; ++i)
	{
		display_DrawCircle(x0, y0, rad-i, color);
	}
}

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
void display_DrawCircleHelper( int16_t x0, int16_t y0, int16_t rad, uint8_t cornername, uint16_t color)
{
  int16_t f     = 1 - rad;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * rad;
  int16_t x     = 0;
  int16_t y     = rad;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      display_DrawPixel(x0 + x, y0 + y, color);
      display_DrawPixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      display_DrawPixel(x0 + x, y0 - y, color);
      display_DrawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      display_DrawPixel(x0 - y, y0 + x, color);
      display_DrawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      display_DrawPixel(x0 - y, y0 - x, color);
      display_DrawPixel(x0 - x, y0 - y, color);
    }
  }
}


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
void display_FillCircleHelper(int16_t x0, int16_t y0, int16_t rad, uint8_t cornername, int16_t delta, uint16_t color)
{

  int16_t f     = 1 - rad;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * rad;
  int16_t x     = 0;
  int16_t y     = rad;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      display_DrawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      display_DrawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      display_DrawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      display_DrawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

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
void display_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
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
			display_DrawPixel(y0, x0, color);
		}
		else
		{
			display_DrawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
}


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
void display_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
   display_DrawLine(x, y, x, y+h-1, color);
}

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
void display_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
   display_DrawLine(x, y, x+w-1, y, color);
}

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
void display_DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  display_DrawFastHLine(x, y, w, color);
  display_DrawFastHLine(x, y+h-1, w, color);
  display_DrawFastVLine(x, y, h, color);
  display_DrawFastVLine(x+w-1, y, h, color);
}

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
void display_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  for (int16_t i=x; i<x+w; i++)
  {
    display_DrawFastVLine(i, y, h, color);
  }
}
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
void display_DrawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t rad, uint16_t color)
{
  // smarter version
  display_DrawFastHLine(x+rad  , y    , w-2*rad, color); // Top
  display_DrawFastHLine(x+rad  , y+h-1, w-2*rad, color); // Bottom
  display_DrawFastVLine(x    , y+rad  , h-2*rad, color); // Left
  display_DrawFastVLine(x+w-1, y+rad  , h-2*rad, color); // Right
  // draw four corners
  display_DrawCircleHelper(x+rad    , y+rad    , rad, 1, color);
  display_DrawCircleHelper(x+w-rad-1, y+rad    , rad, 2, color);
  display_DrawCircleHelper(x+w-rad-1, y+h-rad-1, rad, 4, color);
  display_DrawCircleHelper(x+rad    , y+h-rad-1, rad, 8, color);
}

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
void display_FillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t rad, uint16_t color)
{
  // smarter version
  display_FillRect(x+rad, y, w-2*rad, h, color);

  // draw four corners
  display_FillCircleHelper(x+w-rad-1, y+rad, rad, 1, h-2*rad-1, color);
  display_FillCircleHelper(x+rad    , y+rad, rad, 2, h-2*rad-1, color);
}

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
void display_DrawTriangle(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color)
{
  display_DrawLine(x0, y0, x1, y1, color);
  display_DrawLine(x1, y1, x2, y2, color);
  display_DrawLine(x2, y2, x0, y0, color);
}

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
void display_FillTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
 {

	int16_t a, b, y, last;

	int16_t dx01, dy01, dx02, dy02 , dx12, dy12;
	int32_t sa = 0, sb = 0;


	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1)
	{
		emb_Swap(y0, y1);
		emb_Swap(x0, x1);
	}
	if (y1 > y2)
	{
		emb_Swap(y2, y1);
		emb_Swap(x2, x1);
	}
	if (y0 > y1)
	{
		emb_Swap(y0, y1);
		emb_Swap(x0, x1);
	}

	if (y0 == y2)
	{ // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;
		display_DrawFastHLine(a, y0, b - a + 1, color);
		return;
	}

	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;
	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)
		last = y1;   // Include y1 scanline
	else
		last = y1 - 1; // Skip it

	for (y = y0; y <= last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
		 a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		 b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		 */
		if (a > b)
			emb_Swap(a, b);
		display_DrawFastHLine(a, y, b - a + 1, color);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for (; y <= y2; y++)
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
		 a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		 b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		 */
		if (a > b)
			emb_Swap(a, b);
		display_DrawFastHLine(a, y, b - a + 1, color);
	}
}

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
void display_FillScreen(uint16_t color)
{
  display_FillRect(0, 0, display_handler.width, display_handler.height, color);
}

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
void display_DrawBitmap( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg)
{

  int16_t i, j, byteWidth = (w + 7) / 8;

  for(j=0; j<h; j++)
  {
    for(i=0; i<w; i++ )
    {
      if(emb_ReadConstByte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7)))
      {
        display_DrawPixel(x+i, y+j, color);
      }
      else
      {
      	display_DrawPixel(x+i, y+j, bg);
      }
    }
  }
}

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
void display_DrawXBitmap( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color )
{

  int16_t i, j, byteWidth = (w + 7) / 8;

  for(j=0; j<h; j++)
  {
    for(i=0; i<w; i++ )
    {
      if(emb_ReadConstByte(bitmap + j * byteWidth + i / 8) & (1 << (i % 8)))
      {
        display_DrawPixel(x+i, y+j, color);
      }
    }
  }
}

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
void display_DrawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{

  if((x >= display_handler.width)    || // Clip right
     (y >= display_handler.height)   || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

	for (int8_t i = 0; i < 6; i++)
	{
		uint8_t line;
		if (i == 5)
			line = 0x0;
		else
			line = emb_ReadConstByte(fontTable5x7 + (c * 5) + i);
		for (int8_t j = 0; j < 8; j++)
		{
			if (line & 0x1) {
				if (size == 1) // default size
					display_DrawPixel(x + i, y + j, color);
				else
				{  // big size
					display_FillRect(x + (i * size), y + (j * size), size, size, color);
				}
			}
			else if (bg != color)
			{
				if (size == 1) // default size
					display_DrawPixel(x + i, y + j, bg);
				else
				{  // big size
					display_FillRect(x + i * size, y + j * size, size, size, bg);
				}
			}
			line >>= 1;
		}
	}
}

void display_DrawFontChar(unsigned int c)
{
	uint32_t bitoffset;
	const uint8_t *data;

	//printf("drawFontChar %d\n", c);

	if (c >= display_handler.font->index1_first && c <= display_handler.font->index1_last) {
		bitoffset = c - display_handler.font->index1_first;
		bitoffset *= display_handler.font->bits_index;
	} else if (c >= display_handler.font->index2_first && c <= display_handler.font->index2_last) {
		bitoffset = c - display_handler.font->index2_first + display_handler.font->index1_last - display_handler.font->index1_first + 1;
		bitoffset *= display_handler.font->bits_index;
	} else if (display_handler.font->unicode) {
		return; // TODO: implement sparse unicode
	} else {
		return;
	}
	//printf("  index =  %d\n", fetchbits_unsigned(font->index, bitoffset, font->bits_index));
	data = display_handler.font->data + fetchbits_unsigned(display_handler.font->index, bitoffset, display_handler.font->bits_index);

	uint32_t encoding = fetchbits_unsigned(data, 0, 3);
	if (encoding != 0) return;
	uint32_t width = fetchbits_unsigned(data, 3, display_handler.font->bits_width);
	bitoffset = display_handler.font->bits_width + 3;
	uint32_t height = fetchbits_unsigned(data, bitoffset, display_handler.font->bits_height);
	bitoffset += display_handler.font->bits_height;
	//printf("  size =   %d,%d\n", width, height);

	int32_t xoffset = fetchbits_signed(data, bitoffset, display_handler.font->bits_xoffset);
	bitoffset += display_handler.font->bits_xoffset;
	int32_t yoffset = fetchbits_signed(data, bitoffset, display_handler.font->bits_yoffset);
	bitoffset += display_handler.font->bits_yoffset;
	//printf("  offset = %d,%d\n", xoffset, yoffset);

	uint32_t delta = fetchbits_unsigned(data, bitoffset, display_handler.font->bits_delta);
	bitoffset += display_handler.font->bits_delta;
	//printf("  delta =  %d\n", delta);

	//printf("  cursor = %d,%d\n", cursor_x, cursor_y);

	// horizontally, we draw every pixel, or none at all
	if (display_handler.cursor_x < 0) display_handler.cursor_x = 0;
	int32_t origin_x = display_handler.cursor_x + xoffset;
	if (origin_x < 0) {
		display_handler.cursor_x -= xoffset;
		origin_x = 0;
	}
	if (origin_x + (int)width > display_handler.width) {
		if (!display_handler.wrap) return;
		origin_x = 0;
		if (xoffset >= 0) {
			display_handler.cursor_x = 0;
		} else {
			display_handler.cursor_x = -xoffset;
		}
		display_handler.cursor_y += display_handler.font->line_space;
	}
	if (display_handler.cursor_y >= display_handler.height) return;
	display_handler.cursor_x += delta;

	// vertically, the top and/or bottom can be clipped
	int32_t origin_y = display_handler.cursor_y + display_handler.font->cap_height - height - yoffset;
	//printf("  origin = %d,%d\n", origin_x, origin_y);

	// TODO: compute top skip and number of lines
	int32_t linecount = height;
	//uint32_t loopcount = 0;
	uint32_t y = origin_y;
	while (linecount) {
		//printf("    linecount = %d\n", linecount);
		uint32_t b = fetchbit(data, bitoffset++);
		if (b == 0) {
			//println("    single line");
			uint32_t x = 0;
			do {
				uint32_t xsize = width - x;
				if (xsize > 32) xsize = 32;
				uint32_t bits = fetchbits_unsigned(data, bitoffset, xsize);
				drawFontBits(bits, xsize, origin_x + x, y, 1);
				bitoffset += xsize;
				x += xsize;
			} while (x < width);
			y++;
			linecount--;
		} else {
			uint32_t n = fetchbits_unsigned(data, bitoffset, 3) + 2;
			bitoffset += 3;
			uint32_t x = 0;
			do {
				uint32_t xsize = width - x;
				if (xsize > 32) xsize = 32;
				//printf("    multi line %d\n", n);
				uint32_t bits = fetchbits_unsigned(data, bitoffset, xsize);
				drawFontBits(bits, xsize, origin_x + x, y, n);
				bitoffset += xsize;
				x += xsize;
			} while (x < width);
			y += n;
			linecount -= n;
		}
	}
}

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
void display_Print( unsigned char const *text)
{
	int j = 0;
	//int16_t tempCursorX = display_handler.cursor_x;
	for(int i = 0; text[i] != '\0'/*< textSize*/; ++i, ++j)
	{
		/*
		if(text[i] == '\n')
		{
			display_handler.cursor_y += display_handler.textSize*9;
			j = -1;
			display_handler.cursor_x = 0;
		}
		else
		{
			display_WriteChar(text[i]);
			tempCursorX = display_handler.cursor_x  + (j*(display_handler.textSize*6));
			display_DrawChar(tempCursorX, display_handler.cursor_y, text[i], display_handler.textColor, display_handler.textBGColor, display_handler.textSize);
		}
		*/
		display_WriteChar(text[i]);
	}
	//display_handler.cursor_x = tempCursorX;

}

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
void display_SetTextCursor(int16_t x, int16_t y)
{
	display_handler.cursor_x = x;
	display_handler.cursor_y = y;
}

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
void display_GetTextCursor(int16_t *x, int16_t *y)
{
	*x = display_handler.cursor_x;
	*y = display_handler.cursor_y;
}

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
void display_SetTextSize(uint8_t size)
{
	display_handler.textSize = (size > 0) ? size : 1;
}

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
void display_SetTextColor(uint16_t color)
{
	display_handler.textColor   = color;
}

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
void display_SetTextBGColor(uint16_t color)
{
	display_handler.textBGColor = color;
}


uint16_t display_GetTextColor(void)
{
	return display_handler.textColor;
}

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
void display_SetTextWrap(uint8_t w)
{
	display_handler.wrap = w;
}


/*
void drawChart( BaseType_t xX0, BaseType_t xY0,
				BaseType_t xWidth, BaseType_t xHeight,
				BaseType_t xScale_x, BaseType_t xScale_y,
				BaseType_t xOrigin_x, BaseType_t xOrigin_y,
				BaseType_t *xPoints,
				BaseType_t *xN_points)
{
	BaseType_t xY_interval_n = xHeight/10;
	xS
	drawFastHLine( xX0, int16_t y,
					 int16_t w, uint16_t color);
}
*/

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
void display_SetRotation(uint8_t x)
 {
	display_handler.rotation = (x & 3);
	switch (display_handler.rotation)
	{
	case 0:
	case 1:
	{
		emb_Swap(display_handler.width, display_handler.height);
	}
	break;
	case 2:
	case 3:
	{
		emb_Swap(display_handler.width, display_handler.height);
	}
	break;
	}

	display_hal_SetRotation(display_handler.rotation);
}

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
uint8_t display_GetRotation(void)
{
  return display_handler.rotation;
}



// Return the size of the display (per current ucRotation)
int16_t display_Width(void)
{
  return display_handler.width;
}

int16_t display_Height(void)
{
  return display_handler.height;
}

void display_Invert(uint8_t i)
{

}

/*END: PUBLIC FUNCTIONS*/
/*=======================================================================================*/

/***************************************************************************************
 * END: Module - display.c
 ***************************************************************************************/
