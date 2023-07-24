
/* Self header */
#include "services.h"

/** Utils */
#include "libraries/delay/delay.h"

/*******************************************************************************
 * Locals
 ******************************************************************************/

/*!< Information to create new big number characters, stored in non-volatile memory.*/
uint8_t _bigNumsCodes[] =
{
	0b00000001, // 0
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000001,
	0b00011111,
	0b00000000,
	0b00011111, // 1
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00000000,
	0b00011111, // 2
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000000,
	0b00000001, // 3
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000000,
	0b00011111, // 4
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00011111,
	0b00000000,
	0b00011111, // 5
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000001,
	0b00000001,
	0b00011111,
	0b00000000,
	0b00011111, // 6
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00011111, // 7
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00011111,
	0b00000000
};

/*!< Commands to print big numbers in screen.*/
static const uint8_t _bigNumCommands[10][4] =
{
		{0x01, 0x02, 0x4C, 0x00}, // nr. 0
		{0x20, 0x7C, 0x20, 0x7C}, // nr. 1
		{0x04, 0x05, 0x4C, 0x5F}, // nr. 2
		{0x06, 0x05, 0x5F, 0x00}, // nr. 3
		{0x4C, 0x00, 0x20, 0x03}, // nr. 4
		{0x07, 0x04, 0x5F, 0x00}, // nr. 5
		{0x07, 0x04, 0x4C, 0x00}, // nr. 5
		{0x06, 0x02, 0x20, 0x03}, // nr. 7
		{0x07, 0x05, 0x4C, 0x00}, // nr. 8
		{0x07, 0x05, 0x20, 0x03}  // nr. 9
};

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* waiting macros */
#define Waitns(x) \
		Delay_Waitns(x)				 /* Wait x ns */
#define Waitus(x) \
		Delay_Waitus(x)				 /* Wait x us */
#define Waitms(x) \
		Delay_Waitms(x)				 /* Wait x ms */

#ifndef __FREERTOS_H
#define LcdEnterMutex(x) (void)0
#define LcdExitMutex(x) (void)0
#else
#ifdef LCD_REENTRANT_ACCESS
/* macros for mutex access and release */
#define LcdEnterMutex(x) xSemaphoreTake(x->lcdAccessMutex, portMAX_DELAY)
#define LcdExitMutex(x) xSemaphoreGive(x->lcdAccessMutex)
#endif /* LCD_REENTRANT_ACCESS */
#endif /* __FREERTOS_H */

/*******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * @brief Clear the LCD screen.
 *
 * @param handle the specific LCD handle.
 */
void LCD_Clear(lcdHandle_t *handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	// Clear display, set cursor position to zero
	LCD_Command(handle, LCD_CLEAR_DISPLAY);

	// This command takes a long time!
	Waitms(32);

	LcdExitMutex(handle);
}

/**
 * @brief Set the cursor to the LCD home.
 *
 * @param handle the specific LCD handle.
 */
void LCD_Home(lcdHandle_t *handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	// Set cursor position to zero
	LCD_Command(handle, LCD_RETURN_HOME);

	// This command takes a long time!
	Waitms(32);  

	LcdExitMutex(handle);
}

/**
 * @brief Set the cursor to a specific position.
 *
 * @param handle the specific LCD handle.
 * @param col    the cursor column number.
 * @param row    the cursor row number.
 */
void LCD_SetCursor(lcdHandle_t *handle, uint8_t col, uint8_t row)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	lcdHandle_t* lcdHandle = (lcdHandle_t*)handle;

	const uint8_t max_lines = sizeof(lcdHandle->row_offsets) / sizeof(*lcdHandle->row_offsets);
	if (row >= max_lines)
	{
		// We count rows starting w/0
		row = max_lines - 1;
	}
	if (row >= lcdHandle->config->lines)
	{
		// We count rows starting w/0
		row = lcdHandle->config->lines - 1;
	}

	LCD_Command(handle, LCD_SET_DD_RAM_ADDR | (col + lcdHandle->row_offsets[row]));

	LcdExitMutex(handle);
}


/**
 * @brief Turn the display off.
 *
 * @param handle the specific LCD handle.
 */
void LCD_NoDisplay(lcdHandle_t *handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	lcdHandle_t* lcdHandle = (lcdHandle_t*)handle;
	lcdHandle->display_control &= ~LCD_DISPLAY_ON;
	LCD_Command(handle, LCD_DISPLAY_CONTROL | lcdHandle->display_control);

	LcdExitMutex(handle);
}

/**
 * @brief Turn the display on.
 *
 * @param handle the specific LCD handle.
 */
void LCD_Display(lcdHandle_t *handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	lcdHandle_t* lcdHandle = (lcdHandle_t*)handle;
	lcdHandle->display_control |= LCD_DISPLAY_ON;
	LCD_Command(handle, LCD_DISPLAY_CONTROL | lcdHandle->display_control);

	LcdExitMutex(handle);
}

/**
 * @brief Turns the underline cursor off.
 *
 * @param handle the specific LCD handle.
 */
void LCD_NoCursor(lcdHandle_t *handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	lcdHandle_t* lcdHandle = (lcdHandle_t*)handle;
	lcdHandle->display_control &= ~LCD_CURSOR_ON;
	LCD_Command(handle, LCD_DISPLAY_CONTROL | lcdHandle->display_control);

	LcdExitMutex(handle);
}

/**
 * @brief Turns the underline cursor on.
 *
 * @param handle the specific LCD handle.
 */
void LCD_Cursor(lcdHandle_t *handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	lcdHandle_t* lcdHandle = (lcdHandle_t*)handle;
	lcdHandle->display_control |= LCD_CURSOR_ON;
	LCD_Command(handle, LCD_DISPLAY_CONTROL | lcdHandle->display_control);

	LcdExitMutex(handle);
}

/**
 * @brief Turn off the blinking cursor.
 *
 * @param handle the specific LCD handle.
 */
void LCD_NoBlink(lcdHandle_t *handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	lcdHandle_t* lcdHandle = (lcdHandle_t*)handle;
	lcdHandle->display_control &= ~LCD_BLINK_ON;
	LCD_Command(handle, LCD_DISPLAY_CONTROL | lcdHandle->display_control);

	LcdExitMutex(handle);
}

/**
 * @brief Turn on the blinking cursor.
 *
 * @param handle the specific LCD handle.
 */
void LCD_Blink(lcdHandle_t *handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	lcdHandle_t* lcdHandle = (lcdHandle_t*)handle;
	lcdHandle->display_control |= LCD_BLINK_ON;
	LCD_Command(handle, LCD_DISPLAY_CONTROL | lcdHandle->display_control);

	LcdExitMutex(handle);
}

/**
 * @brief Scroll the display to the left
 *        without changing the RAM.
 *
 * @param handle the specific LCD handle.
 */
void LCD_ScrollDisplayLeft(lcdHandle_t *handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	LCD_Command(handle, LCD_CURSOR_SHIFT | LCD_DISPLAY_MOVE | LCD_MOVE_LEFT);

	LcdExitMutex(handle);
}

/**
 * @brief Scroll the display to the right
 *        without changing the RAM.
 *
 * @param handle the specific LCD handle.
 */
void LCD_ScrollDisplayRight(lcdHandle_t *handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	LCD_Command(handle, LCD_CURSOR_SHIFT | LCD_DISPLAY_MOVE | LCD_MOVE_RIGHT);

	LcdExitMutex(handle);
}

/**
 * @brief This is for text that flows Left to Right.
 *
 * @param handle the specific LCD handle.
 */
void LCD_LeftToRight(lcdHandle_t *handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	lcdHandle_t* lcdHandle = (lcdHandle_t*)handle;
	lcdHandle->display_mode |= LCD_ENTRY_LEFT;
	LCD_Command(handle, LCD_ENTRY_MODE_SET | lcdHandle->display_mode);

	LcdExitMutex(handle);
}

/**
 * @brief This is for text that flows Right to Left.
 *
 * @param handle the specific LCD handle.
 */
void LCD_RightToLeft(lcdHandle_t *handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	lcdHandle_t* lcdHandle = (lcdHandle_t*)handle;
	lcdHandle->display_mode &= ~LCD_ENTRY_LEFT;
	LCD_Command(handle, LCD_ENTRY_MODE_SET | lcdHandle->display_mode);

	LcdExitMutex(handle);
}

/**
 * @brief This will 'right justify' text from the cursor.
 *
 * @param handle the specific LCD handle.
 */
void LCD_Autoscroll(lcdHandle_t *handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	lcdHandle_t* lcdHandle = (lcdHandle_t*)handle;
	lcdHandle->display_mode |= LCD_ENTRY_SHIFT_INCREMENT;
	LCD_Command(handle, LCD_ENTRY_MODE_SET | lcdHandle->display_mode);

	LcdExitMutex(handle);
}

/**
 * @brief This will 'left justify' text from the cursor.
 *
 * @param handle the specific LCD handle.
 */
void LCD_NoAutoscroll(lcdHandle_t *handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	lcdHandle_t* lcdHandle = (lcdHandle_t*)handle;
	lcdHandle->display_mode &= ~LCD_ENTRY_SHIFT_INCREMENT;
	LCD_Command(handle, LCD_ENTRY_MODE_SET | lcdHandle->display_mode);

	LcdExitMutex(handle);
}

/**
 * @brief Allows us to fill the first 8 CGRAM locations
 *        with custom characters.
 *
 * @param handle   the specific LCD handle.
 * @param location the CGRAM location.
 * @param charmap  the custom character mapped in a matrix.
 */
void LCD_CreateChar(lcdHandle_t *handle, uint8_t location, uint8_t charmap[])
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	// We only have 8 locations 0-7
	location &= 0x7;
	LCD_Command(handle, LCD_SET_CG_RAM_ADDR | (location << 3));
	for (int i = 0; i < 8; ++i)
	{
		LCD_Write(handle, charmap[i]);
	}

	LcdExitMutex(handle);
}

/**
 * @brief Write a string in the current LCD cursor position.
 *
 * @param handle the specific LCD handle.
 * @param str    the string buffer.
 */
void LCD_WriteString(lcdHandle_t *handle, char *str)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(handle);

	while (*str != '\0')
	{
		LCD_Write(handle, *str);
		++str;
	}

	LcdExitMutex(handle);
}

/**
 * @brief Send the big numbers bit map to the HD44780 controller.
 *
 * @note This function must be called before to use the LCD_WriteBigNum
 *       function.
 *
 * @param handle the specific LCD handle.
 */
void LCD_CreateBigNumsChars(lcdHandle_t *handle)
{
	uint8_t j = 0;

	for(uint8_t i = 0; i < 8; ++i)
	{
		LCD_CreateChar(handle, i, &_bigNumsCodes[j]);
		j += 8;
	}
}

/**
 * @brief Print a number in the "big number" format.
 *
 * @note This function can be called only after the call of the
 *       LCD_CreateBigNumsChars function.
 *
 * @param handle the specific LCD handle.
 * @param col    the column where to print the number.
 * @param num    the number from 0 to 9 to be printed.
 */
void LCD_WriteBigNum(lcdHandle_t *handle, uint8_t col, uint8_t num)
{
	LCD_SetCursor(handle, col, 0);
	LCD_Write(handle, _bigNumCommands[num][0]);
	LCD_Write(handle, _bigNumCommands[num][1]);
	LCD_SetCursor(handle, col, 1);
	LCD_Write(handle, _bigNumCommands[num][2]);
	LCD_Write(handle, _bigNumCommands[num][3]);
}
