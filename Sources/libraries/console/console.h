/**
 * @file	console.h
 * @author  Matheus Leitzke Pinto <matheus.pinto@ifsc.edu.br>
 * @version 1.0
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
 * A generic console library for write/read characters.
 */

#ifndef LIBRARIES_CONSOLE_H_
#define LIBRARIES_CONSOLE_H_

#include <libraries/stream/stream.h>
#include <libraries/printf/printf.h>
#include <libraries/delay/delay.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/*!
 * @addtogroup console
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!< Defines if console instances will be created statically.
 *   If commented, console instances will be allocated dynamically in heap. */
#define CONSOLE_STATIC_OBJECTS_CREATION
/*!< The number of object instances that will be created statically.*/
#define CONSOLE_MAX_STATIC_OBJECTS 1
/*!< Define the console to be reentrant. So it will creates a mutex to
 * protect the console access to race conditions*/
#define CONSOLE_IS_REENTRANT
/*!< If console ANSI compliable, it will define specific methods.*/
#define CONSOLE_IS_ANSI
/*!< The maximum length in bytes of the buffer that hold the string that
 * represents the number converted in "Console_PrintNum" or "Console_PrintFloat".*/
#define CONSOLE_MAX_NUMBER_BUFFER_LEN (15U)
/*!< Comment macros below to exclude the corresponding function from compile. */
#define CONSOLE_PRINTLN_FUNCTION


typedef enum
{
	CONSOLE_COLOR_BLACK = 30U,	//Black color
	CONSOLE_COLOR_RED, 			//Red color
	CONSOLE_COLOR_GREEN, 		//Green color
	CONSOLE_COLOR_YELLOW, 		//Yellow color
	CONSOLE_COLOR_BLUE, 		//Blue color
	CONSOLE_COLOR_MAGENTA, 		//Magenta color
	CONSOLE_COLOR_CYAN, 		//Cyan color
	CONSOLE_COLOR_WHITE, 		//White color
	CONSOLE_COLOR_DEFAULT = 39U
}consoleColor_t;

typedef enum
{
	CONSOLE_CURSOR_TO_UP = 30U,	//Move cursor to up
	CONSOLE_CURSOR_TO_DOWN, 	//Move cursor to down
	CONSOLE_CURSOR_TO_LEFT, 	//Move cursor to left
	CONSOLE_CURSOR_TO_RIGHT, 	//Move cursor to right
}consoleCursor_t;


/*!
 * @brief console manager configuration structure
 *
 * This structure holds the configuration settings for the console module.
 */
typedef struct
{
	/*!< The stream handle.*/
	streamHandle_t stream;
	/*!< The new line feed command string (ex.: "\n", "\r\n", etc).*/
	const unsigned char *newLine;
}consoleConfig_t;

/*!< The handle that must be passed to the API to communicate with specific console module.*/
typedef void* consoleHandle_t;

/*!< A internally use function for printf purpose.*/
void outPrintf( char character, void* arg );

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief A console printf implementation.
 *
 * @param handle - The console handle.
 * @param format - A string that specifies the format of the output.
 * @param ...    - A list of values.
 *
 * @return - The number of characters printed, or a negative value
 *           if there was an output error.
 *
 */
#define Console_Printf( handle, ... ) fctprintf( outPrintf, handle, __VA_ARGS__ )

/**
 * @brief Creates the structure to configure the console instance.
 *
 * @return - The configuration structure or;
 *         - NULL, if was not possible to create the structure.
 *
 */
consoleConfig_t* Console_CreateConfig(void);

/**
 * @brief Initialize the console module.
 *
 * @param config - The configurations structure.
 *
 * @return - The specific console module handle that must be passed
 *           to the API for communication or;
 *         - NULL, if was not possible to create the handle.
 *
 */
consoleHandle_t Console_Init(consoleConfig_t *config);


/**
 * @brief Print a string.
 *
 * @param handle - The console handle.
 * @param str - A null terminate string.
 *
 */
void Console_Print(consoleHandle_t handle, const char *str);

/**
 * @brief Scan a number of characters in the console and
 *        stores in a null terminate string.
 *
 * @param handle - The console handle.
 * @param str - A buffer to stores the string.
 * @param length - The number of characters to scan.
 *
 */
void Console_Scan(consoleHandle_t handle, char *str, size_t length);

#ifdef CONSOLE_PRINTLN_FUNCTION
/**
 * @brief Print a string plus the new line command (ex.: '\n', \r', etc).
 *
 * @param handle - The console handle.
 * @param str - A null terminate string.
 *
 */
void Console_Println(consoleHandle_t handle, const char *str);
#endif

/**
 * @brief Scan a line in the console and
 *        stores in a null terminate string.
 *
 * @param handle - The console handle.
 * @param str - A buffer to stores the line plus a null terminate string.
 *
 */
void Console_Scanln(consoleHandle_t handle, char *str);

/**
 * @brief Get a character.
 *
 * @param handle - The console handle.
 *
 * @return - The character.
 *
 */
char Console_GetChar( consoleHandle_t handle );

/**
 * @brief Put a character.
 *
 * @param handle - The console handle.
 *
 * @return - The character.
 *
 */
void Console_PutChar( consoleHandle_t handle, char ch );

/**
 * @brief Sends a integer number as text to terminal.
 *
 * @param handle - The console handle.
 * @param num  - the integer number.
 * @param base - the numerical system that the num will be showed:
 * 		      \a 2 - binary;
 * 			  \a 10 - decimal;
 * 			  \a 16 - hex.
 *
 */
void Console_PrintNum( consoleHandle_t handle, int32_t num, uint8_t base );

/**
 * @brief Sends a float number as text to terminal.
 *
 * @param handle - The console handle.
 * @param num  - the integer number.
 * @param afterPoint - number of digits to be considered after the point.
 *
 */
void Console_PrintFloat( consoleHandle_t handle, float num, int afterPoint );

/**
 * @brief Play the console bell sound.
 *
 * @param handle - The console handle.
 *
 */
void Console_PlayBell( consoleHandle_t handle );

#ifdef CONSOLE_IS_ANSI

/**
 * @brief Clear the console screen.
 *
 * @param handle - The console handle.
 *
 */
void Console_Clear( consoleHandle_t handle );

/**
 * @brief Set the console cursor to a specific position.
 *
 * @param handle - The console handle.
 * @param direction - Up, down, left or right as defined in the
 *                    enumerated type.
 * @param intervals - If up or down, it specifies the number of lines.
 *                    If left or right, it specifies the number of columns.
 *
 */
void Console_SetCursor( consoleHandle_t handle, consoleCursor_t direction, uint8_t intervals );

/**
 * @brief Move to a specific x and y position.
 *
 * @param handle - The console handle.
 * @param x, y - cartesian position.
 *
 */
void Console_MoveTo( consoleHandle_t handle, uint8_t x, uint8_t y );

/**
 * @brief Erase all characters of current line.
 *
 * @param handle - The console handle.
 *
 */
void Console_EraseLine( consoleHandle_t handle );

/**
 * @brief Enter the character attribute mode.
 *
 * 		  After call this function, only methods
 * 		  starting with "Console_SetChar" should be called.
 * 		  To exit this mode, call "Console_ExitCharAttrMode".
 *
 * @param handle - The console handle.
 *
 */
void Console_EnterCharAttrMode( consoleHandle_t handle );

/**
 * @brief Exit the character attribute mode.
 *
 * 		  Only calls this method if "Console_EnterCharAttrMode"
 * 		  was called.
 *
 * @param handle - The console handle.
 *
 */
void Console_ExitCharAttrMode( consoleHandle_t handle );

/**
 * @brief Set the text color.
 *
 * 		  Only calls this method if "Console_EnterCharAttrMode"
 * 		  was called.
 *
 * @param handle - The console handle.
 * @param foreground - The foreground color.
 * @param background - The background color.
 *
 */
void Console_SetCharColor(consoleHandle_t handle, consoleColor_t foreground, consoleColor_t background);

/**
 * @brief Set the text to bold.
 *
 * 		  Only calls this method if "Console_EnterCharAttrMode"
 * 		  was called.
 *
 * @param handle - The console handle.
 *
 */
void Console_SetCharBold( consoleHandle_t handle );

/**
 * @brief Set the text to normal.
 *
 * 		  Only calls this method if "Console_EnterCharAttrMode"
 * 		  was called.
 *
 * @param handle - The console handle.
 *
 */
void Console_SetCharNormal( consoleHandle_t handle );

/**
 * @brief Set the text to underlined.
 *
 * 		  Only calls this method if "Console_EnterCharAttrMode"
 * 		  was called.
 *
 * @param handle - The console handle.
 *
 */
void Console_SetCharUnderlined( consoleHandle_t handle );
#endif // CONSOLE_IS_ANSI

/*! @}*/

#endif /* LIBRARIES_CONSOLE_H_ */
