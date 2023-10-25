# Stream

This module is a software library for generic sending/receiveing bytes to/from a stream for embedded systems.

To use the Stream commands, follow these steps:

- Use the [SDK pattern](/README.md) present in this repository to create your project.
- Implement the basic read/write functions using your specific system implementation (e.g.: UART, I2C, SPI, etc). Here is a example:

```c
/* UART Write/Read services to be passed to the Stream instance */

// Returns the number of bytes that you can send.
size_t GetAvailToWrite( void )
{
	return UART_IsTxAvailable( UART0 );
}

// Returns the number of bytes that you can read.
size_t GetBytesToRead( void )
{
	return UART_IsRxAvailable( UART0 );
}

// Write a byte.
void Write( const uint8_t data )
{
	UART_Write( UART0, data );
}

// Read a byte.
uint8_t Read( void )
{
	return UART_Read( UART0 );
}
```

- Creates the Stream configuration instance and pass the read/write implementations.

```c
/* Console service initilization */
uartStreamConfig = Stream_CreateConfig();

uartStreamConfig->GetAvailToWrite = GetAvailToWrite;
uartStreamConfig->GetBytesToRead = GetBytesToRead;
uartStreamConfig->Write = Write;
uartStreamConfig->Read = Read;
```
	
- Initialize the Stream using the created configuration:

```c
streamHandle_t uartStream = Stream_Init( uartStreamConfig );
```

After initialization, you can call the desired functions to control the Stream.

# Definitions

- `STREAM_STATIC_OBJECTS_CREATION`: Defines if stream instances will be created statically. If commented, console instances will be allocated dynamically in heap.
- `STREAM_MAX_STATIC_OBJECTS`: The number of object instances that will be created statically.


# API

The following functions are available:

---
## Stream_CreateConfig

Creates the structure to configure the stream instance.

**Return:**
- The configuration structure, or
- NULL if it was not possible to create the structure.

---
## Stream_Init

Initialize the stream module.

**Parameters:**
- `config`: The variable with the configurations defined.

**Return:**
- The specific stream module handler that must be passed to the API for communication, or
- NULL if it was not possible to create the handler.

---
## Stream_WriteBlocking

Write data to the stream module with a predetermined length.

**Parameters:**
- `handle`: The specific stream handle.
- `data`: A pointer to the message buffer.
- `length`: The message length.

---
## Stream_ReadBlocking

Read a message with a predetermined length.

**Parameters:**
- `handle`: The specific stream handle.
- `data`: A pointer to the message buffer.
- `length`: The message length.

---
## Stream_Write

Write data to the stream module with a predetermined length.

**Parameters:**
- `handle`: The specific stream handle.
- `data`: A pointer to the message buffer.
- `length`: The message length.

**Return:**
- `SYSTEM_STATUS_SUCCESS` if the message was completely written to the stream, or
- `SYSTEM_STATUS_FAIL` if there is no space in the output buffer.

---
## Stream_Read

Read a message with a predetermined length.

**Parameters:**
- `handle`: The specific stream handle.
- `data`: A pointer to the message buffer.
- `length`: The message length.

**Return:**
- `SYSTEM_STATUS_SUCCESS` if the message was completely read from the stream, or
- `SYSTEM_STATUS_FAIL` if there are not enough bytes to read.

---
## Stream_GetAvailToWrite

Get the available number of bytes in the output buffer that are available to write.

**Parameters:**
- `handle`: The specific stream handle.

**Return:**
- The number of bytes in the output buffer that are available to write.

---
## Stream_GetBytesToRead

Get the number of bytes in the input buffer that are available to read.

**Parameters:**
- `handle`: The specific stream handle.

**Return:**
- The number of bytes in the input buffer that are available to read.
---



# Example 
This example initialize a Stream application using UART to send/receveive text.

```c
#include "mcu/drivers/port/port.h"
#include "mcu/drivers/uart/uart.h"
#include "libraries/stream/stream.h"
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
	
	/* Microcontroller specific hardware initilization */
	PORT_Init( PORTB );
	PORT_SetMux( PORTB, 1, PORT_MUX_ALT2 );
	PORT_SetMux( PORTB, 2, PORT_MUX_ALT2 );

	UART_SetClkSrc( UART0, UART_CLOCK_FLL );
	UART_Init( UART0, 115200, UART_TX_RX_ENABLE, UART_NO_PARITY, UART_ONE_STOP_BIT );

	/* Stream service initilization */
	uartStreamConfig = Stream_CreateConfig();
	SYSTEM_ASSERT( uartStreamConfig );

	uartStreamConfig->GetAvailToWrite = GetAvailToWrite;
	uartStreamConfig->GetBytesToRead = GetBytesToRead;
	uartStreamConfig->Write = Write;
	uartStreamConfig->Read = Read;

	uartStream = Stream_Init( uartStreamConfig );
	SYSTEM_ASSERT( uartStream ); // Verify if was returned a NULL pointer

	int i = 0;
	char buffer[MAX_BUFFER_SIZE];

	/* Perfoms a echo program */
	for ( ; ; )
	{
		/* Read a text until end of line ('\n')*/
		Stream_ReadBlocking( uartStream, &buffer[i], 1 );
		if ( buffer[i] == '\n' )
		{
			/* Resend the received string (echo)*/
			for (i = 0; buffer[i] != '\n'; ++i)
			{
				Stream_WriteBlocking( uartStream, &buffer[i], 1 );
			}
			Stream_WriteBlocking( uartStream, &buffer[i], 1 );
			i = 0; /* Clear the counter for the next reading */
		}
		else
			++i;

	}
}

```