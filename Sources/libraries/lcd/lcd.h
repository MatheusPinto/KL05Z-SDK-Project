/**
 * @file	lcd.h
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

#ifndef LCD_HD44780_H_
#define LCD_HD44780_H_

/** General config */
#include <mcu/common.h>

/** MCU specific LCD interface */
#include "lcd_mcu_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup lcd
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!< HD44780 control commands.*/
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_RETURN_HOME 0x02
#define LCD_ENTRY_MODE_SET 0x04
#define LCD_DISPLAY_CONTROL 0x08
#define LCD_CURSOR_SHIFT 0x10
#define LCD_FUNCTION_SET 0x20
#define LCD_SET_CG_RAM_ADDR 0x40
#define LCD_SET_DD_RAM_ADDR 0x80

/*!< Flags for display entry mode.*/
#define LCD_ENTRY_RIGHT 0x00
#define LCD_ENTRY_LEFT 0x02
#define LCD_ENTRY_SHIFT_INCREMENT 0x01
#define LCD_ENTRY_SHIFT_DECREMENT 0x00

/*!< Flags for display on/off control.*/
#define LCD_DISPLAY_ON 0x04
#define LCD_DISPLAY_OFF 0x00
#define LCD_CURSOR_ON 0x02
#define LCD_CURSOR_OFF 0x00
#define LCD_BLINK_ON 0x01
#define LCD_BLINK_OFF 0x00

/*!< Flags for display/cursor shift.*/
#define LCD_DISPLAY_MOVE 0x08
#define LCD_CURSOR_MOVE 0x00
#define LCD_MOVE_RIGHT 0x04
#define LCD_MOVE_LEFT 0x00

/*!< Flags for function set.*/
#define LCD_4_BIT_MODE 0x00
#ifndef LCD_4_BIT_MODE
	#define LCD_8_BIT_MODE 0x10
#endif
#define LCD_2_LINE 0x08
#define LCD_1_LINE 0x00
#define LCD_5x10_DOTS 0x04
#define LCD_5x8_DOTS 0x00

/*!< Flags for backlight control */
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

/* !< Defines if LCD instances will be created statically.
 *    If commented, LCD instances will be allocated dynamically in heap.
 */
#define LCD_STATIC_OBJECTS_CREATION
#ifdef LCD_STATIC_OBJECTS_CREATION
	#define LCD_MAX_STATIC_OBJECTS 1 /*!< The number of object instances that will be created statically.*/
#endif

/*!< Uncomment this macro if want to use reentrant access of API.*/
#define LCD_REENTRANT_ACCESS

/*!< If is not used RTOS or mutex, then mutex control functions will be bypassed.*/
#ifndef _LCD_USE_RTOS
#define LcdEnterMutex(x) (void)0
#define LcdExitMutex(x) (void)0
#else
#ifdef LCD_REENTRANT_ACCESS
/* macros for mutex access and release */
#define LcdEnterMutex(x) LCD_RTOS_ENTER_MUTEX(x)
#define LcdExitMutex(x) LCD_RTOS_EXIT_MUTEX(x)
#endif /* LCD_REENTRANT_ACCESS */
#endif /* __FREERTOS_H */

/*******************************************************************************
 * Types
 ******************************************************************************/

/*!< Hardware configuration structure describing current display hardware connections */
typedef void* lcdAdapter_t;

/*!< LCD Handle for API use */
typedef void* lcdHandle_t;

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*!< Enumeration to identify if byte sent is a command or data.*/
enum
{
	LCD_COMMAND_MODE = 0,
	LCD_DATA_MODE    = 1
};

/*!< Defines all possible hardware adapters when creating a new lcd */
typedef enum
{
	LCD_PARALLEL_HARD_ADAPTER,
	LCD_I2C_HARD_ADAPTER
} lcdHardwareAdapters_t;

/**
 * @brief Structure that holds the LCD bus information
 */
typedef struct
{
	/*!< Hardware adapter type being used */
	lcdHardwareAdapters_t type;

	/*!< Mid level command callback */
	void (*write)(lcdAdapter_t adapter, uint8_t value, uint8_t is_expanded, uint8_t mode);

	/*!< Bus set rs callback */
	void (*setRs)(lcdAdapter_t adapter);

	/*!< Bus clear rs callback */
	void (*clrRs)(lcdAdapter_t adapter);

	/*!< Bus set enable callback */
	void (*setEn)(lcdAdapter_t adapter);

	/*!< Bus clear enable callback */
	void (*clrEn)(lcdAdapter_t adapter);
} lcdAdapterInterface_t;

/*******************************************************************************
 * Functions
 ******************************************************************************/

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
);

/**
 * @brief Clear the LCD screen.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Clear(lcdHandle_t handle);

/**
 * @brief Set the cursor to the LCD home.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Home(lcdHandle_t handle);

/**
 * @brief Set the cursor to a specific position.
 *
 * @param handle - the specific LCD handle.
 * @param col    - the cursor column number.
 * @param row    - the cursor row number.
 */
void LCD_SetCursor(lcdHandle_t handle, uint8_t col, uint8_t row);

/**
 * @brief Turn the display off.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_NoDisplay(lcdHandle_t handle);

/**
 * @brief Turn the display on.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Display(lcdHandle_t handle);

/**
 * @brief Turns the underline cursor off.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_NoCursor(lcdHandle_t handle);

/**
 * @brief Turns the underline cursor on.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Cursor(lcdHandle_t handle);

/**
 * @brief Turn off the blinking cursor.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_NoBlink(lcdHandle_t handle);

/**
 * @brief Turn on the blinking cursor.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Blink(lcdHandle_t handle);

/**
 * @brief Scroll the display to the left
 *        without changing the RAM.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_ScrollDisplayLeft(lcdHandle_t handle);

/**
 * @brief Scroll the display to the right
 *        without changing the RAM.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_ScrollDisplayRight(lcdHandle_t handle);

/**
 * @brief This is for text that flows Left to Right.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_LeftToRight(lcdHandle_t handle);

/**
 * @brief This is for text that flows Right to Left.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_RightToLeft(lcdHandle_t handle);

/**
 * @brief This will 'right justify' text from the cursor.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Autoscroll(lcdHandle_t handle);

/**
 * @brief This will 'left justify' text from the cursor.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_NoAutoscroll(lcdHandle_t handle);

/**
 * @brief Allows us to fill the first 8 CGRAM locations
 *        with custom characters.
 *
 * @param handle   - the specific LCD handle.
 * @param location - the CGRAM location.
 * @param charmap  - the custom character mapped in a matrix.
 */
void LCD_CreateChar(lcdHandle_t handle, uint8_t location, const uint8_t charmap[]);

/**
 * @brief Write a string in the current LCD cursor position.
 *
 * @param handle - the specific LCD handle.
 * @param str    - the string buffer.
 */
void LCD_WriteString(lcdHandle_t handle, char *str);

/**
 * @brief Send the big numbers bit map to the HD44780 controller.
 *
 * @note This function must be called before to use the LCD_WriteBigNum
 *       function.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_CreateBigNumsChars(lcdHandle_t handle);

/**
 * @brief Print a number in the "big number" format.
 *
 * @note This function can be called only after the call of the
 *       LCD_CreateBigNumsChars function.
 *
 * @param handle - the specific LCD handle.
 * @param col    - the column where to print the number.
 * @param num    - the number from 0 to 9 to be printed.
 */
void LCD_WriteBigNum(lcdHandle_t handle, uint8_t col, uint8_t num);

/**
 * @brief Send a command to the HD44780 controller.
 *
 * This function is used used inside API. It should
 * be avoided in favor of other API functions. The user
 * must call this function only if it is necessary to
 * make another kind of communication not covered to
 * the API.
 *
 * @param handle - the specific LCD handle.
 * @param value  - the command value.
 */
void LCD_Command(lcdHandle_t handle, uint8_t value);

/**
 * @brief Send a data to the HD44780 controller.
 *
 * This function is used used inside API. It should
 * be avoided in favor of other API functions. The user
 * must call this function only if it is necessary to
 * make another kind of communication not covered to
 * the API.
 *
 * @param handle - the specific LCD handle.
 * @param value  - the data value.
 */
void LCD_Write(lcdHandle_t handle, uint8_t value);

#ifdef __cplusplus
}  /* extern "C" */
#endif

/*! @}*/

#endif /* LCD_HD44780_H_ */
