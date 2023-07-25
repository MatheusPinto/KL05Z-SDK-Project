/**
 * @file	stream.h
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
 * A generic stream library for write/read bytes.
 */

#ifndef LIBRARIES_STREAM_H_
#define LIBRARIES_STREAM_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "system/status_code.h"

/*!
 * @addtogroup stream
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!< Defines if stream instances will be created statically.
 *   If commented, stream instances will be allocated dynamically in heap. */
#define STREAM_STATIC_OBJECTS_CREATION
#define STREAM_MAX_STATIC_OBJECTS 1 /*!< The number of object instances that will be created statically.*/

/*!
 * @brief stream manager configuration structure
 *
 * This structure holds the configuration settings for the stream module.
 */
typedef struct
{
	/*!< Function pointer that gets the number of bytes free to sent in the output buffer.*/
	size_t ( *GetAvailToWrite )( void );
	/*!< Function pointer that gets the number of bytes not read in input buffer.*/
	size_t ( *GetBytesToRead )( void );
	/*!< Function pointer to write a byte in the stream output.*/
	void ( *Write )( const uint8_t );
	/*!< Function pointer to read a byte in the stream input.*/
	uint8_t ( *Read )( void );
	/*!< The new line feed command string (ex.: "\n", "\r\n", etc).*/
	const unsigned char *newLine;
}streamConfig_t;


/*!< The handle that must be passed to the API to communicate with specific stream module.*/
typedef void* streamHandle_t;


/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Creates the structure to configure the stream instance.
 *
 * @return - The configuration structure or;
 *         - NULL, if was not possible to create the structure.
 *
 */
streamConfig_t* Stream_CreateConfig( void );

/**
 * @brief Initialize the stream module.
 *
 * @param config - the variable with the configurations defined.
 *
 * @return - The specific stream module handler that must be passed
 *           to the API for communication or;
 *         - NULL, if was not possible to create the handler.
 *
 */
streamHandle_t Stream_Init( streamConfig_t *config );

/**
 * @brief Write a data in the stream module with a
 *        predetermined length.
 *
 *        The function will pooling until all
 *        the message is sent.
 *
 * @param handle - the specific stream handle.
 * @param data - a pointer to the message buffer.
 * @param length - the message length.
 *
 */
void Stream_WriteBlocking( streamHandle_t handle, const uint8_t *data, size_t length );

/**
 * @brief Read a message with a predetermined length.
 *
 *        The function will pooling execution until all
 *        the message is received.
 *
 * @param handle - the specific stream handle.
 * @param msg - a pointer to the message buffer.
 * @param length - the message length.
 *
 */
void Stream_ReadBlocking( streamHandle_t handle, uint8_t *data, size_t length );

/**
 * @brief Write a data in the stream module with a
 *        predetermined length.
 *
 *        The function will sent the message only if
 *        the output buffer has space. Otherwise,
 *        it will return.
 *
 * @param handle - the specific stream handle.
 * @param data - a pointer to the message buffer.
 * @param length - the message length.
 *
 * @return - SYSTEM_STATUS_SUCCESS, if message was complete write to the stream, or;
 *           SYSTEM_STATUS_FAIL, if there is no space in the output buffer.
 *
 */
uint8_t Stream_Write( streamHandle_t handle, const uint8_t *data, size_t length );

/**
 * @brief Read a message with a predetermined length.
 *
 *		  The function will receive the message only if
 *        the input buffer has a message with the desired size.
 *        Otherwise, it will return.
 *
 * @param handle - the specific stream handle.
 * @param msg - a pointer to the message buffer.
 * @param length - the message length.
 *
 * @return - SYSTEM_STATUS_SUCCESS, if message was complete read from the stream, or;
 *           SYSTEM_STATUS_FAIL, if there is no enough bytes to read.
 *
 */
uint8_t Stream_Read( streamHandle_t handle, uint8_t *data, size_t length );

/**
 * @brief Get the available number of bytes in the output buffer that
 *        available to write.
 *
 * @param handle - the specific stream handle.
 *
 * @return - The number of bytes in the output buffer that
 *           available to write.
 *
 */
size_t Stream_GetAvailToWrite( streamHandle_t handle );

/**
 * @brief Get the number of bytes in the input buffer that
 *        available to read.
 *
 * @param handle - the specific stream handle.
 *
 * @return - The number of bytes in the input buffer that
 *        	 available to read.
 *
 */
size_t Stream_GetBytesToRead( streamHandle_t handle );

/*! @}*/

#endif /* LIBRARIES_STREAM_H_ */
