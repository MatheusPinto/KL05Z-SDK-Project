# Console

This module is a software library for sending/receiveing characters to/from a console/terminal for embedded systems. It provides a set of ANSI compliant functions such as coloring, bolding and many other services for text customization.

To use the Console commands, follow these steps:

- Use the [SDK pattern](/README.md) present in this repository to create your project.
- It is necessary the ["Util"](/Sources/libraries/Util/README.md) and ["Stream"](/MatheusPinto/KL05Z-SDK-Project/tree/main/Sources/libraries/stream/README.md) libraries to use this API.
- If you wanna to use `printf` function, it is necessary [this](/Sources/libraries/printf) module.
- Initialize a [stream](/Sources/libraries/stream/README.md) instance with the read/write services for the specific console system (e.g.: UART, USB stack, etc).
	
- Create the Console configuration instance using the following syntax:
```c
consoleConfig_t *terminalConfig = Console_CreateConfig();
```
- Initialize the Console using the created configuration and the specified parameters:

```c
terminalConfig->stream = myStream; // The stream instance must be created in advance.
terminalConfig->newLine = "\n";    // A null-terminated string. Other newLine commands could be "\r" and "\r\n"

consoleHandle_t terminal = Console_Init( terminalConfig );
```

After initialization, you can call the desired functions to control the Console.

# Definitions

- `CONSOLE_STATIC_OBJECTS_CREATION`: Defines if console instances will be created statically. If commented, console instances will be allocated dynamically in heap.
- `CONSOLE_MAX_STATIC_OBJECTS`: The number of object instances that will be created statically.
- `CONSOLE_IS_REENTRANT`: Define the console to be reentrant. So it will creates a mutex to protect the console access to race conditions
- `CONSOLE_IS_ANSI`: If console ANSI compliable, it will define specific methods.
- `CONSOLE_MAX_NUMBER_BUFFER_LEN`: The maximum length in bytes of the buffer that hold the string that represents the number converted in "Console_PrintNum" or "Console_PrintFloat".


# API

The following functions are available:

## Console_CreateConfig

Creates the structure to configure the console instance.

**Return:**

The configuration structure or NULL, if it was not possible to create the structure.

---

## Console_Init

Initialize the console module.

**Parameters:**

- `config`: The configurations structure.

**Return:**

The specific console module handle that must be passed to the API for communication or NULL, if it was not possible to create the handle.

---

## Console_Print

Print a string.

**Parameters:**

- `handle`: The console handle.
- `str`: A null-terminated string.

---

## Console_Scan

Scan a number of characters in the console and store them in a null-terminated string.

**Parameters:**

- `handle`: The console handle.
- `str`: A buffer to store the string.
- `length`: The number of characters to scan.

---

## Console_Println (available only if CONSOLE_PRINTLN_FUNCTION is defined)

Print a string followed by a newline command (ex.: '\n', '\r', etc).

**Parameters:**

- `handle`: The console handle.
- `str`: A null-terminated string.

---

## Console_Scanln

Scan a line in the console and store it in a null-terminated string.

**Parameters:**

- `handle`: The console handle.
- `str`: A buffer to store the line and the null-terminated string.

---

## Console_Printf

A console printf implementation.

**Parameters:**

- `handle`: The console handle.
- `format`: A string that specifies the format of the output.
- `...`: A list of values.

**Return:**

The number of characters printed, or a negative value if there was an output error.

---

## Console_GetChar

Get a character.

**Parameters:**

- `handle`: The console handle.

**Return:**

The character.

---

## Console_PutChar

Put a character.

**Parameters:**

- `handle`: The console handle.
- `ch`: The character to put.

---

## Console_PrintNum

Send an integer number as text to the terminal.

**Parameters:**

- `handle`: The console handle.
- `num`: The integer number.
- `base`: The numerical system in which the number will be shown:
  - 2: binary;
  - 10: decimal;
  - 16: hexadecimal.

---

## Console_PrintFloat

Send a float number as text to the terminal.

**Parameters:**

- `handle`: The console handle.
- `num`: The float number.
- `afterPoint`: The number of digits to consider after the decimal point.

---

## Console_PlayBell

Play the console bell sound.

**Parameters:**

- `handle`: The console handle.

---

## Console_Clear (available only if CONSOLE_IS_ANSI is defined)

Clear the console screen.

**Parameters:**

- `handle`: The console handle.

---

## Console_SetCursor (available only if CONSOLE_IS_ANSI is defined)

Set the console cursor to a specific position.

**Parameters:**

- `handle`: The console handle.
- `direction`: Up, down, left, or right as defined in the enumerated type.
- `intervals`: If up or down, it specifies the number of lines. If left or right, it specifies the number of columns.

---

## Console_MoveTo (available only if CONSOLE_IS_ANSI is defined)

Move to a specific x and y position.

**Parameters:**

- `handle`: The console handle.
- `x`, `y`: Cartesian position.

---

## Console_EraseLine (available only if CONSOLE_IS_ANSI is defined)

Erase all characters of the current line.

**Parameters:**

- `handle`: The console handle.

---

## Console_EnterCharAttrMode (available only if CONSOLE_IS_ANSI is defined)

Enter the character attribute mode.

After calling this function, only methods starting with "Console_SetChar" should be called.
To exit this mode, call "Console_ExitCharAttrMode".

**Parameters:**

- `handle`: The console handle.

---

## Console_ExitCharAttrMode (available only if CONSOLE_IS_ANSI is defined)

Exit the character attribute mode.

Only call this method if "Console_EnterCharAttrMode" was called.

**Parameters:**

- `handle`: The console handle.

---

## Console_SetCharColor (available only if CONSOLE_IS_ANSI is defined)

Set the text color.

Only call this method if "Console_EnterCharAttrMode" was called.

**Parameters:**

- `handle`: The console handle.
- `foreground`: The foreground color.
- `background`: The background color.

---

## Console_SetCharBold (available only if CONSOLE_IS_ANSI is defined)

Set the text to bold.

Only call this method if "Console_EnterCharAttrMode" was called.

**Parameters:**

- `handle`: The console handle.

---

## Console_SetCharNormal (available only if CONSOLE_IS_ANSI is defined)

Set the text to normal.

Only call this method if "Console_EnterCharAttrMode" was called.

**Parameters:**

- `handle`: The console handle.

---

## Console_SetCharUnderlined (available only if CONSOLE_IS_ANSI is defined)

Set the text to underlined.

Only call this method if "Console_EnterCharAttrMode" was called.

**Parameters:**

- `handle`: The console handle.



# Example 
This example initialize a Console application with a UART Stream and send/receveive a set of text/commands.

```c
#include <stdint.h>
#include "mcu/drivers/port/port.h"
#include "mcu/drivers/uart/uart.h"
#include "libraries/console/console.h"
#include "system/assert.h"

#define MAX_BUFFER_SIZE 60

/* UART Write/Read services to be passed to the Stream instance */

size_t GetAvailToWrite( void )
{
	return UART_IsTxAvailable( UART0 );
}

size_t GetBytesToRead( void )
{
	return UART_IsRxAvailable( UART0 );
}

void Write( const uint8_t data )
{
	UART_Write( UART0, data );
}

uint8_t Read( void )
{
	return UART_Read( UART0 );
}

int main(void)
{
	streamConfig_t *uartStreamConfig;
	streamHandle_t uartStream;
	consoleConfig_t *terminalConfig;
	consoleHandle_t terminal;

	/* Microcontroller specific hardware initilization */
	PORT_Init( PORTB );
	PORT_SetMux( PORTB, 1, PORT_MUX_ALT2 );
	PORT_SetMux( PORTB, 2, PORT_MUX_ALT2 );

	UART_SetClkSrc( UART0, UART_CLOCK_FLL );
	UART_Init( UART0, 115200, UART_TX_RX_ENABLE, UART_NO_PARITY, UART_ONE_STOP_BIT );

	/* Console service initilization */
	uartStreamConfig = Stream_CreateConfig();
	SYSTEM_ASSERT( uartStreamConfig );

	uartStreamConfig->GetAvailToWrite = GetAvailToWrite;
	uartStreamConfig->GetBytesToRead = GetBytesToRead;
	uartStreamConfig->Write = Write;
	uartStreamConfig->Read = Read;

	uartStream = Stream_Init( uartStreamConfig );
	SYSTEM_ASSERT( uartStream ); // Verify if was returned a NULL pointer

	terminalConfig = Console_CreateConfig();
	SYSTEM_ASSERT( terminalConfig ); // Verify if was returned a NULL pointer

	terminalConfig->stream = uartStream;
	terminalConfig->newLine = "\n";

	terminal = Console_Init( terminalConfig );
	SYSTEM_ASSERT( terminal ); // Verify if was returned a NULL pointer

	int i = 0;
	char buffer[MAX_BUFFER_SIZE];

	/* Console service use */
	Console_Scanln( terminal, buffer );
	Console_Println( terminal, "You entered a new line command!" );

	for ( ; ; )
	{
		/* Wait for a line text */
		Console_Scanln( terminal, buffer );
		Console_Clear( terminal ); 		  // Clear the terminal screen
		Console_MoveTo( terminal, 0, 0 ); // Go to origin
		Console_Println( terminal, buffer ); // Echo the text
		/* Printf has high memory footprint. Use it with caution! */
		//Console_Printf( terminal, "Printf OK!!! %d\n", 666 );
		//Console_Printf( terminal, "Again!!! %f\n", 0.378 );
		//Console_Printf( terminal, "And again... %c\n", buffer[0]);

		/* Enter in ANSI character command mode */
		/* Set the printing text with the bold red color, background black */
		Console_EnterCharAttrMode( terminal );
		Console_SetCharColor( terminal, CONSOLE_COLOR_RED, CONSOLE_COLOR_BLACK );
		Console_SetCharBold( terminal );
		Console_ExitCharAttrMode( terminal );

		Console_Println( terminal, buffer );

		/* Enter in ANSI character command mode */
		/* Set the printing text with the underlined blue color, background with default color */
		Console_EnterCharAttrMode( terminal );
		Console_SetCharColor( terminal, CONSOLE_COLOR_BLUE, CONSOLE_COLOR_DEFAULT );
		Console_SetCharUnderlined( terminal );
		Console_ExitCharAttrMode( terminal );

		Console_Println( terminal, buffer );

		/* Enter in ANSI character command mode */
		/* Set the printing text with the underlined, bold yellow color, background blue */
		Console_EnterCharAttrMode( terminal );
		Console_SetCharColor( terminal, CONSOLE_COLOR_YELLOW, CONSOLE_COLOR_BLUE );
		Console_SetCharBold( terminal );
		Console_SetCharUnderlined( terminal );
		Console_ExitCharAttrMode( terminal );
		Console_Println( terminal, buffer );

		/* Enter in ANSI character command mode */
		/* Set the printing text with normal console parameters */
		Console_EnterCharAttrMode( terminal );
		Console_SetCharNormal( terminal );
		Console_ExitCharAttrMode( terminal );

		/* Printing many numbers... */
		Console_PrintNum( terminal, 666, 10 );
		Console_Println( terminal, "" );
		Console_Print( terminal, "0x" );
		Console_PrintNum( terminal, 0xFF3, 16 );
		Console_Println( terminal, "" );
		Console_PrintFloat( terminal, 3.156211, 4 );
		Console_Println( terminal, "" );

		Console_PlayBell( terminal );
	}
}

```