#ifndef LCD_HD44780_COMMANDS_H_
#define LCD_HD44780_COMMANDS_H_

/* LCD */
#include "../lcd.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup lcd
 * @{
 */

/**
 * @brief Clear the LCD screen.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Clear(lcdHandle_t *handle);

/**
 * @brief Set the cursor to the LCD home.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Home(lcdHandle_t *handle);

/**
 * @brief Set the cursor to a specific position.
 *
 * @param handle - the specific LCD handle.
 * @param col    - the cursor column number.
 * @param row    - the cursor row number.
 */
void LCD_SetCursor(lcdHandle_t *handle, uint8_t col, uint8_t row);

/**
 * @brief Turn the display off.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_NoDisplay(lcdHandle_t *handle);

/**
 * @brief Turn the display on.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Display(lcdHandle_t *handle);

/**
 * @brief Turns the underline cursor off.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_NoCursor(lcdHandle_t *handle);

/**
 * @brief Turns the underline cursor on.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Cursor(lcdHandle_t *handle);

/**
 * @brief Turn off the blinking cursor.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_NoBlink(lcdHandle_t *handle);

/**
 * @brief Turn on the blinking cursor.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Blink(lcdHandle_t *handle);

/**
 * @brief Scroll the display to the left
 *        without changing the RAM.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_ScrollDisplayLeft(lcdHandle_t *handle);

/**
 * @brief Scroll the display to the right
 *        without changing the RAM.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_ScrollDisplayRight(lcdHandle_t *handle);

/**
 * @brief This is for text that flows Left to Right.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_LeftToRight(lcdHandle_t *handle);

/**
 * @brief This is for text that flows Right to Left.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_RightToLeft(lcdHandle_t *handle);

/**
 * @brief This will 'right justify' text from the cursor.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Autoscroll(lcdHandle_t *handle);

/**
 * @brief This will 'left justify' text from the cursor.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_NoAutoscroll(lcdHandle_t *handle);

/**
 * @brief Allows us to fill the first 8 CGRAM locations
 *        with custom characters.
 *
 * @param handle   - the specific LCD handle.
 * @param location - the CGRAM location.
 * @param charmap  - the custom character mapped in a matrix.
 */
void LCD_CreateChar(lcdHandle_t *handle, uint8_t location, uint8_t charmap[]);

/**
 * @brief Write a string in the current LCD cursor position.
 *
 * @param handle - the specific LCD handle.
 * @param str    - the string buffer.
 */
void LCD_WriteString(lcdHandle_t *handle, char *str);

/**
 * @brief Send the big numbers bit map to the HD44780 controller.
 *
 * @note This function must be called before to use the LCD_WriteBigNum
 *       function.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_CreateBigNumsChars(lcdHandle_t *handle);

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
void LCD_WriteBigNum(lcdHandle_t *handle, uint8_t col, uint8_t num);

#ifdef __cplusplus
}  /* extern "C" */
#endif

/*! @}*/

#endif /* !LCD_HD44780_COMMANDS_H_ */
