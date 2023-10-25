/**
 * @file	lcd.c
 * @author  Matheus Leitzke Pinto <matheus.pinto@ifsc.edu.br>
 * @version 2.0
 * @date    2023
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * A library for LCD with HD44780 controller.
 */

/** Self header */
#include "lcd.h"

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*!< Enumeration to identify objects creation.*/
enum
{
	LCD_OBJECT_IS_HANDLE,
	LCD_OBJECT_IS_CONFIG
};

/*******************************************************************************
 * Structures
 ******************************************************************************/

/*!
 * @brief LCD manager configuration structure
 *
 * This structure holds the configuration settings for the LCD module.
 */
typedef struct
{
	/*!< The display columns number. */
	uint8_t cols;

	/*!< The display lines number. */
	uint8_t lines;

	/*!< The display character size in pixels. */
	uint8_t char_size;

	/*!< The LCD bus. */
	lcdAdapter_t adapter;
} lcdConfig_t;

/*!
 * @brief LCD handle structure used internally
 */
typedef struct
{
	/*!< The pointer to the configuration structure passed by the user.*/
	lcdConfig_t *config;

	/*!< The display function command that will be send to the LCD controller.*/
	uint8_t display_function;

	/*!< The display control command that will be send to the LCD controller.*/
	uint8_t display_control;

	/*!< The display mode command that will be send to the LCD controller.*/
	uint8_t display_mode;

	/*!< The row offset.Get(12);s list determined by the number of LCD rows and columns.*/
	uint8_t row_offsets[4];

#ifdef __FREERTOS_H
#ifdef LCD_REENTRANT_ACCESS
	/*!< The mutex used for mutual exclusion in API calls.*/
	lcdRTOSSemaphore_t lcdAccessMutex;
#endif /* LCD_REENTRANT_ACCESS */
#endif /* __FREERTOS_H */
} lcdPrivateHandle_t;


/*******************************************************************************
 * Locals
 ******************************************************************************/

#ifdef LCD_STATIC_OBJECTS_CREATION

/*!< The static list of configuration structures that is returned to the LCD API.*/
static lcdConfig_t g_lcdConfigList[LCD_MAX_STATIC_OBJECTS];

/*!< The static list of handle structures that is returned to the LCD API.*/
static lcdPrivateHandle_t g_lcdHandleList[LCD_MAX_STATIC_OBJECTS];

/*!< The number of configuration and handle structures created using the LCD API.*/
static uint8_t g_staticConfigsCreated;
static uint8_t g_staticHandlesCreated;

#endif

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
 * Forward declarations
 ******************************************************************************/

/**
 * @brief Set the row offset, which is based from the LCD module used.
 *
 * @param handle - the specific LCD handle.
 *
 */
static void SetRowOffsets(lcdPrivateHandle_t *handle);

/**
 * @brief Create an specific object used by an LCD instance.
 *
 * @param objectType - \a K_LCD_OBJECT_IS_HANDLE, if want to create a LCD handle;
 * 					   \a K_LCD_OBJECT_IS_CONFIG, if want to create a LCD configuration structure.
 *
 */
static void* CreateObject(uint8_t objectType);

/**
 * @brief Destroy an specific object used by an LCD instance.
 * 
 * @param objectType - \a K_LCD_OBJECT_IS_HANDLE, if want to destroy a LCD handle;
 * 					   \a K_LCD_OBJECT_IS_CONFIG, if want to destroy a LCD configuration structure.
 * @param object Object to be destroyed.
 */
void DestroyObject(uint8_t objectType, void *object);

/*******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * @brief Create an specific object used by an LCD instance.
 *
 * @param objectType - \a K_LCD_OBJECT_IS_HANDLE, if want to create a LCD handle;
 * 					   \a K_LCD_OBJECT_IS_CONFIG, if want to create a LCD configuration structure.
 *
 */
static void* CreateObject(uint8_t objectType)
{
	void* objectCreated = NULL;
#ifdef LCD_STATIC_OBJECTS_CREATION
	switch (objectType)
	{
	case LCD_OBJECT_IS_HANDLE:
		if(g_staticHandlesCreated < LCD_MAX_STATIC_OBJECTS)
		{
			objectCreated = (void*)&g_lcdHandleList[g_staticHandlesCreated++];
		}
		break;
	case LCD_OBJECT_IS_CONFIG:
		if(g_staticConfigsCreated < LCD_MAX_STATIC_OBJECTS)
		{
			objectCreated = (void*)&g_lcdConfigList[g_staticConfigsCreated++];
		}
		break;
	}
#else
	switch (objectType)
	{
	case LCD_OBJECT_IS_HANDLE:
		objectCreated = embUtil_Malloc(sizeof(lcdHandle_t));
		break;
	case LCD_OBJECT_IS_CONFIG:
		objectCreated = embUtil_Malloc(sizeof(lcdConfig_t));
		break;
	}
#endif
	return objectCreated;
}

/**
 * @brief Destroy an specific object used by an LCD instance.
 * 
 * @param objectType - \a K_LCD_OBJECT_IS_HANDLE, if want to destroy a LCD handle;
 * 					   \a K_LCD_OBJECT_IS_CONFIG, if want to destroy a LCD configuration structure.
 * @param object Object to be destroyed.
 */
void DestroyObject(uint8_t objectType, void *object)
{
#ifdef LCD_STATIC_OBJECTS_CREATION
	switch (objectType)
	{
	case LCD_OBJECT_IS_HANDLE:
		if(g_staticHandlesCreated)
			--g_staticHandlesCreated;
		break;
	case LCD_OBJECT_IS_CONFIG:
		if(g_staticConfigsCreated)
			--g_staticConfigsCreated;
		break;
	}
#else
	embUtil_Free(obj);
#endif

	object = NULL;
}

/**
 * @brief Initialize the LCD module.
 *
 * @param adapter - Hardware adapter for this handle
 * @param cols - Display columns number
 * @param lines - Display lines number
 * @param char_size - Display character size
 *
 * @return - The specific LCD module handle that must be passed
 *           to the API for communication or;
 *         - NULL, if was not possible to create the handle.
 */
lcdHandle_t LCD_Init(
	lcdAdapter_t adapter, uint8_t cols, uint8_t lines, uint8_t char_size
)
{
	SYSTEM_ASSERT( adapter );

	lcdPrivateHandle_t *handle = CreateObject(LCD_OBJECT_IS_HANDLE);
	lcdConfig_t *config = CreateObject(LCD_OBJECT_IS_CONFIG);

	/** Reinterpret as hardware config interface */
	lcdAdapterInterface_t *adapter_interface = (lcdAdapterInterface_t*)adapter;

	/** TODO add free */
	if(!handle || !config) return NULL;

	/** Config setup */
	config->adapter = adapter;
	config->cols = cols;
	config->lines = lines;
	config->char_size = char_size;
	handle->config = config;

#ifdef LCD_4_BIT_MODE
	handle->display_function = LCD_4_BIT_MODE | LCD_1_LINE | LCD_5x8_DOTS;
#else
	handle->display_function = LCD_8_BIT_MODE | LCD_1_LINE | LCD_5x8_DOTS;
#endif

	if (config->lines > 1)
	{
		handle->display_function |= LCD_2_LINE;
	}

	SetRowOffsets(handle);

	// for some 1 line displays you can select a 10 pixel high font
	if ((config->char_size != LCD_5x8_DOTS) && (config->lines == 1)) {
		handle->display_function |= LCD_5x10_DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. So we'll wait 50ms.
	Waitms(50);

	// Pull both RS and R/W low to begin commands
	if (adapter_interface->type == LCD_I2C_HARD_ADAPTER)
	{
		// Reset expander and turn backlight off (Bit 8 = 1)
		adapter_interface->write(handle, LCD_BACKLIGHT, true, LCD_COMMAND_MODE);
		Waitms(1000);
	}
	else
	{
		adapter_interface->clrRs(adapter_interface);
		adapter_interface->clrEn(adapter_interface);
	}

	//put the LCD into 4 bit or 8 bit mode
#ifdef LCD_4_BIT_MODE
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46
	// we start in 8bit mode, try to set 4 bit mode
	adapter_interface->write(adapter_interface, 0x03, false, LCD_COMMAND_MODE);
	Waitus(4500); // wait min 4.1ms
	// second try
	adapter_interface->write(adapter_interface, 0x03, false, LCD_COMMAND_MODE);
	Waitus(4500); // wait min 4.1ms
	// third go!
	adapter_interface->write(adapter_interface, 0x03, false, LCD_COMMAND_MODE);
	Waitus(4500);
	// finally, set to 4-bit interface
	adapter_interface->write(adapter_interface, 0x02, false, LCD_COMMAND_MODE);
	Waitus(150);
#else
	// this is according to the hitachi HD44780 datasheet
	// page 45 figure 23

	// Send function set command sequence
	LCD_Command((lcdHandle_t)handle, LCD_FUNCTION_SET | display_function);
	Waitus(4500); // wait min 4.1ms

	// second try
	LCD_Command((lcdHandle_t)handle, LCD_FUNCTION_SET | display_function);
	Waitus(150);

	// third go
	LCD_Command((lcdHandle_t)handle, LCD_FUNCTION_SET | display_function);
#endif
	// finally, set # lines, font size, etc.
	LCD_Command(handle, LCD_FUNCTION_SET | handle->display_function);

	// turn the display on with no cursor or blinking default
	handle->display_control = LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF;
	LCD_Display(handle);

	// clear it off
	LCD_Clear(handle);

	Waitms(10);

	// Initialize to default text direction (for romance languages)
	handle->display_mode = LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT;
	// set the entry mode
	LCD_Command(handle, LCD_ENTRY_MODE_SET | handle->display_mode);

	Waitus(400);

	return handle;
}

/**
 * @brief Clear the LCD screen.
 *
 * @param handle the specific LCD handle.
 */
void LCD_Clear(lcdHandle_t handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	// Clear display, set cursor position to zero
	LCD_Command(handle, LCD_CLEAR_DISPLAY);

	// This command takes a long time!
	Waitms(32);

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief Set the cursor to the LCD home.
 *
 * @param handle the specific LCD handle.
 */
void LCD_Home(lcdHandle_t handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	// Set cursor position to zero
	LCD_Command(handle, LCD_RETURN_HOME);

	// This command takes a long time!
	Waitms(32);

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief Set the cursor to a specific position.
 *
 * @param handle the specific LCD handle.
 * @param col    the cursor column number.
 * @param row    the cursor row number.
 */
void LCD_SetCursor(lcdHandle_t handle, uint8_t col, uint8_t row)
{
	SYSTEM_ASSERT(handle);

	/** Reinterpret as private handle */
	lcdPrivateHandle_t* lcdHandle = handle;
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

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

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}


/**
 * @brief Turn the display off.
 *
 * @param handle the specific LCD handle.
 */
void LCD_NoDisplay(lcdHandle_t handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	/** Reinterpret as private handle */
	lcdPrivateHandle_t* lcdHandle = handle;
	lcdHandle->display_control &= ~LCD_DISPLAY_ON;
	LCD_Command(handle, LCD_DISPLAY_CONTROL | lcdHandle->display_control);

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief Turn the display on.
 *
 * @param handle the specific LCD handle.
 */
void LCD_Display(lcdHandle_t handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	/** Reinterpret as private handle */
	lcdPrivateHandle_t* lcdHandle = handle;
	lcdHandle->display_control |= LCD_DISPLAY_ON;
	LCD_Command(handle, LCD_DISPLAY_CONTROL | lcdHandle->display_control);

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief Turns the underline cursor off.
 *
 * @param handle the specific LCD handle.
 */
void LCD_NoCursor(lcdHandle_t handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	/** Reinterpret as private handle */
	lcdPrivateHandle_t* lcdHandle = handle;
	lcdHandle->display_control &= ~LCD_CURSOR_ON;
	LCD_Command(handle, LCD_DISPLAY_CONTROL | lcdHandle->display_control);

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief Turns the underline cursor on.
 *
 * @param handle the specific LCD handle.
 */
void LCD_Cursor(lcdHandle_t handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	/** Reinterpret as private handle */
	lcdPrivateHandle_t* lcdHandle = handle;
	lcdHandle->display_control |= LCD_CURSOR_ON;
	LCD_Command(handle, LCD_DISPLAY_CONTROL | lcdHandle->display_control);

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief Turn off the blinking cursor.
 *
 * @param handle the specific LCD handle.
 */
void LCD_NoBlink(lcdHandle_t handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	/** Reinterpret as private handle */
	lcdPrivateHandle_t* lcdHandle = handle;
	lcdHandle->display_control &= ~LCD_BLINK_ON;
	LCD_Command(handle, LCD_DISPLAY_CONTROL | lcdHandle->display_control);

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief Turn on the blinking cursor.
 *
 * @param handle the specific LCD handle.
 */
void LCD_Blink(lcdHandle_t handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	/** Reinterpret as private handle */
	lcdPrivateHandle_t* lcdHandle = handle;
	lcdHandle->display_control |= LCD_BLINK_ON;
	LCD_Command(handle, LCD_DISPLAY_CONTROL | lcdHandle->display_control);

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief Scroll the display to the left
 *        without changing the RAM.
 *
 * @param handle the specific LCD handle.
 */
void LCD_ScrollDisplayLeft(lcdHandle_t handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	LCD_Command(handle, LCD_CURSOR_SHIFT | LCD_DISPLAY_MOVE | LCD_MOVE_LEFT);

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief Scroll the display to the right
 *        without changing the RAM.
 *
 * @param handle the specific LCD handle.
 */
void LCD_ScrollDisplayRight(lcdHandle_t handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	LCD_Command(handle, LCD_CURSOR_SHIFT | LCD_DISPLAY_MOVE | LCD_MOVE_RIGHT);

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief This is for text that flows Left to Right.
 *
 * @param handle the specific LCD handle.
 */
void LCD_LeftToRight(lcdHandle_t handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	/** Reinterpret as private handle */
	lcdPrivateHandle_t* lcdHandle = handle;
	lcdHandle->display_mode |= LCD_ENTRY_LEFT;
	LCD_Command(handle, LCD_ENTRY_MODE_SET | lcdHandle->display_mode);

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief This is for text that flows Right to Left.
 *
 * @param handle the specific LCD handle.
 */
void LCD_RightToLeft(lcdHandle_t handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	/** Reinterpret as private handle */
	lcdPrivateHandle_t* lcdHandle = handle;
	lcdHandle->display_mode &= ~LCD_ENTRY_LEFT;
	LCD_Command(handle, LCD_ENTRY_MODE_SET | lcdHandle->display_mode);

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief This will 'right justify' text from the cursor.
 *
 * @param handle the specific LCD handle.
 */
void LCD_Autoscroll(lcdHandle_t handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	/** Reinterpret as private handle */
	lcdPrivateHandle_t* lcdHandle = handle;
	lcdHandle->display_mode |= LCD_ENTRY_SHIFT_INCREMENT;
	LCD_Command(handle, LCD_ENTRY_MODE_SET | lcdHandle->display_mode);

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief This will 'left justify' text from the cursor.
 *
 * @param handle the specific LCD handle.
 */
void LCD_NoAutoscroll(lcdHandle_t handle)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	/** Reinterpret as private handle */
	lcdPrivateHandle_t* lcdHandle = handle;
	lcdHandle->display_mode &= ~LCD_ENTRY_SHIFT_INCREMENT;
	LCD_Command(handle, LCD_ENTRY_MODE_SET | lcdHandle->display_mode);

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief Allows us to fill the first 8 CGRAM locations
 *        with custom characters.
 *
 * @param handle   the specific LCD handle.
 * @param location the CGRAM location.
 * @param charmap  the custom character mapped in a matrix.
 */
void LCD_CreateChar(lcdHandle_t handle, uint8_t location, const uint8_t charmap[])
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	// We only have 8 locations 0-7
	location &= 0x7;
	LCD_Command(handle, LCD_SET_CG_RAM_ADDR | (location << 3));
	for (int i = 0; i < 8; ++i)
	{
		LCD_Write(handle, charmap[i]);
	}

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief Write a string in the current LCD cursor position.
 *
 * @param handle the specific LCD handle.
 * @param str    the string buffer.
 */
void LCD_WriteString(lcdHandle_t handle, char *str)
{
	SYSTEM_ASSERT(handle);
	LcdEnterMutex(((lcdPrivateHandle_t*)handle));

	while (*str != '\0')
	{
		LCD_Write(handle, *str);
		++str;
	}

	LcdExitMutex(((lcdPrivateHandle_t*)handle));
}

/**
 * @brief Send the big numbers bit map to the HD44780 controller.
 *
 * @note This function must be called before to use the LCD_WriteBigNum
 *       function.
 *
 * @param handle the specific LCD handle.
 */
void LCD_CreateBigNumsChars(lcdHandle_t handle)
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
void LCD_WriteBigNum(lcdHandle_t handle, uint8_t col, uint8_t num)
{
	LCD_SetCursor(handle, col, 0);
	LCD_Write(handle, _bigNumCommands[num][0]);
	LCD_Write(handle, _bigNumCommands[num][1]);
	LCD_SetCursor(handle, col, 1);
	LCD_Write(handle, _bigNumCommands[num][2]);
	LCD_Write(handle, _bigNumCommands[num][3]);
}

/**
 * @brief Set the row offset, which is based from the LCD module used.
 *
 * @param handle - the specific LCD handle.
 *
 */
static void SetRowOffsets(lcdPrivateHandle_t *handle)
{
	handle->row_offsets[0] = 0x00;
	handle->row_offsets[1] = 0x40;
	handle->row_offsets[2] = 0x00 + handle->config->cols;
	handle->row_offsets[3] = 0x40 + handle->config->cols;
}

/** MID level commands */

void LCD_Command(lcdHandle_t handle, uint8_t value)
{
	/** Reinterpret as hardware adapter interface */
	lcdAdapterInterface_t *adapter = ((lcdPrivateHandle_t*)handle)->config->adapter;

	/** Call command for current adapter */
	adapter->write(adapter, value, false, LCD_COMMAND_MODE);
}

void LCD_Write(lcdHandle_t handle, uint8_t value)
{
	/** Reinterpret as hardware adapter interface */
	lcdAdapterInterface_t *adapter = ((lcdPrivateHandle_t*)handle)->config->adapter;

	/** Call command for current adapter */
	adapter->write(adapter, value, false, LCD_DATA_MODE);
}
