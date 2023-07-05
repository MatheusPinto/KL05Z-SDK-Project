# LCD HD44780 Commands

The LCD HD44780 module is a software library that enables the initialization and control of LCD HD44780 displays. It provides a comprehensive set of functions for seamless interaction with LCDs connected to embedded systems.
Usage

To use the LCD HD44780 commands, follow these steps:

- Include the services.h file in your project. This file contains all the functions to control the LCD.
- Include the desired adapter. The available adapters can be found in the generic_drivers/lcd/adapters/ directory.
	
- Create the adapter using the following syntax:
```
	lcdAdapter_t adapter = LCD_Create<adapter>(<adapter configuration>);
```
- Initialize the LCD using the created adapter and the specified parameters:

```
lcdHandle_t *lcd = LCD_Init(
	adapter, <rows>, <lines>, <dots>
);

/* Run commands */
```

Call the desired functions to control the LCD.

# Functions

The following functions are available:
```

lcdHandle_t* LCD_Init(lcdAdapter_t *adapter, uint8_t cols, uint8_t lines, uint8_t char_size)

    Description: Initialize the LCD module.
    Inputs:
        adapter: Hardware adapter for this handle.
        cols: Display columns number.
        lines: Display lines number.
        char_size: Display character size.
    Returns: The specific LCD module handle that must be passed to the API for communication or NULL if it was not possible to create the handle.

lcdAdapter_t LCD_CreateI2CAdapter(I2C_Type *base, uint8_t slave_addr)

    Description: Creates an I2C hardware adapter configuration object.
    Inputs:
        base: I2C base memory map.
        slave_addr: LCD slave address.
    Returns: Created I2C LCD hardware configuration object based on the provided parameters.

void LCD_FreeAdapterI2C(lcdAdapter_t adapter)

    Description: Destroys a given LCD I2C adapter.
    Inputs:
        adapter: Adapter to be destroyed.

lcdAdapter_t LCD_CreateParallelAdapter(lcdPin_t data[], lcdPin_t reset, lcdPin_t enable)

    Description: Creates a Parallel hardware adapter configuration object.
    Inputs:
        cols: Number of columns that LCD contains.
        lines: Number of lines that LCD contains.
        char_size: LCD character size.
        data: Data pin array.
        reset: Reset pin.
        enable: Enable pin.
    Returns: Created Parallel LCD hardware configuration object based on the provided parameters.

void LCD_FreeAdapterParallel(lcdAdapter_t adapter)

    Description: Destroys a given LCD parallel adapter.
    Inputs:
        adapter: Adapter to be destroyed.

	void LCD_Clear(lcdHandle_t *handle)
		Description: Clears the LCD screen.
		Inputs:
			handle: The specific LCD handle.

	void LCD_Home(lcdHandle_t *handle)
		Description: Sets the cursor to the home position on the LCD.
		Inputs:
			handle: The specific LCD handle.

	void LCD_SetCursor(lcdHandle_t *handle, uint8_t col, uint8_t row)
		Description: Sets the cursor to a specific position on the LCD.
		Inputs:
			handle: The specific LCD handle.
			col: The cursor column number.
			row: The cursor row number.

	void LCD_NoDisplay(lcdHandle_t *handle)
		Description: Turns off the display on the LCD.
		Inputs:
			handle: The specific LCD handle.

	void LCD_Display(lcdHandle_t *handle)
		Description: Turns on the display on the LCD.
		Inputs:
			handle: The specific LCD handle.

	void LCD_NoCursor(lcdHandle_t *handle)
		Description: Turns off the underline cursor on the LCD.
		Inputs:
			handle: The specific LCD handle.

	void LCD_Cursor(lcdHandle_t *handle)
		Description: Turns on the underline cursor on the LCD.
		Inputs:
			handle: The specific LCD handle.

	void LCD_NoBlink(lcdHandle_t *handle)
		Description: Turns off the blinking cursor on the LCD.
		Inputs:
			handle: The specific LCD handle.

	void LCD_Blink(lcdHandle_t *handle)
		Description: Turns on the blinking cursor on the LCD.
		Inputs:
			handle: The specific LCD handle.

	void LCD_ScrollDisplayLeft(lcdHandle_t *handle)
		Description: Scrolls the display to the left without changing the RAM on the LCD.
		Inputs:
			handle: The specific LCD handle.

	void LCD_ScrollDisplayRight(lcdHandle_t *handle)
		Description: Scrolls the display to the right without changing the RAM on the LCD.
		Inputs:
			handle: The specific LCD handle.

	void LCD_LeftToRight(lcdHandle_t *handle)
		Description: Sets the text flow direction from left to right on the LCD.
		Inputs:
			handle: The specific LCD handle.

	void LCD_RightToLeft(lcdHandle_t *handle)
		Description: Sets the text flow direction from right to left on the LCD.
		Inputs:
			handle: The specific LCD handle.

	void LCD_Autoscroll(lcdHandle_t *handle)
		Description: Enables auto-scrolling of text from the cursor position on the LCD.
		Inputs:
			handle: The specific LCD handle.

	void LCD_NoAutoscroll(lcdHandle_t *handle)
		Description: Disables auto-scrolling of text on the LCD.
		Inputs:
			handle: The specific LCD handle.

	void LCD_CreateChar(lcdHandle_t *handle, uint8_t location, uint8_t charmap[])
		Description: Allows the creation of custom characters at specific CGRAM locations on the LCD.
		Inputs:
			handle: The specific LCD handle.
			location: The CGRAM location where the custom character will be stored.
			charmap: The custom character represented as a matrix.

	void LCD_WriteString(lcdHandle_t *handle, char *str)
		Description: Writes a string at the current LCD cursor position.
		Inputs:
			handle: The specific LCD handle.
			str: The string buffer.

	void LCD_CreateBigNumsChars(lcdHandle_t *handle)
		Description: Sends the big numbers bit map to the HD44780 controller.
		Note: This function must be called before using the LCD_WriteBigNum function.
		Inputs:
			handle: The specific LCD handle.

	void LCD_WriteBigNum(lcdHandle_t *handle, uint8_t col, uint8_t num)
		Description: Prints a number in the "big number" format on the LCD.
		Note: This function can only be called after the LCD_CreateBigNumsChars function.
		Inputs:
			handle: The specific LCD handle.
			col: The column where the number should be printed.
			num: The number (0-9) to be printed.
```

# Example 
This example initialize a i2c lcd 16 x 8 and write "Hello World"

```
#include "generic_drivers/lcd/adapters/lcd_i2c_adapter.h"
#include "generic_drivers/lcd/services/services.h"

int main()
{
	/* Initialize the adapter */
	lcdAdapter_t adapter = LCD_CreateI2CAdapter(I2C0, 0x3f);

	/* Initialize the LCD */
	lcdHandle_t *lcd = LCD_Init(
		adapter, 16, 8, LCD_5x8_DOTS
	);

	/* Clear the lcd */
	LCD_Clear(lcd);

	/* Write Hello World! */
	LCD_WriteString(lcd, "Hello World!");

	return 0;
}
```