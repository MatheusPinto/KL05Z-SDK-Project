/**
 * @file	stream.c
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

#include "system/assert.h"
#include "stream.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifdef STREAM_STATIC_OBJECTS_CREATION
/*!< A list with all stream configuration structures used in application. */
static streamConfig_t g_streamConfigList[STREAM_MAX_STATIC_OBJECTS];
/*!< The number of configuration structures created. */
static uint8_t g_staticConfigsCreated;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/**********************************************************************************/
streamConfig_t* Stream_CreateConfig( void )
{
	streamConfig_t *ret;

#ifdef	STREAM_STATIC_OBJECTS_CREATION
	if ( g_staticConfigsCreated < STREAM_MAX_STATIC_OBJECTS )
	{
		ret = &g_streamConfigList[g_staticConfigsCreated];
	}
	else
	{
		ret = NULL;
	}
#else
	ret = SYSTEM_MALLOC( sizeof ( streamConfig_t ) );
#endif
	return ret;
}

/**********************************************************************************/
streamHandle_t Stream_Init( streamConfig_t *config )
{
	SYSTEM_ASSERT( config );
	return config;
}

/**********************************************************************************/
void Stream_WriteBlocking( streamHandle_t handle, const uint8_t *data, size_t length )
{
	streamConfig_t *config = handle;

	for(int i = 0; i < length; ++i)
	{
		while(!config->GetAvailToWrite());
		config->Write(data[i]);
	}
}

/**********************************************************************************/
void Stream_ReadBlocking( streamHandle_t handle, uint8_t *data, size_t length )
{
	streamConfig_t *config = handle;

	for(int i = 0; i < length; ++i)
	{
		while(!config->GetBytesToRead());
		data[i] = config->Read();
	}
}

/**********************************************************************************/
uint8_t Stream_Read( streamHandle_t handle, uint8_t *data, size_t length )
{
	streamConfig_t *config = handle;
	uint8_t status = SYSTEM_STATUS_FAIL;

	if ( config->GetBytesToRead() == length )
	{
		for(int i = 0; i < length; ++i)
		{
			data[i] = config->Read();
		}
		status = SYSTEM_STATUS_SUCCESS;
	}
	return status;
}

/**********************************************************************************/
uint8_t Stream_Write( streamHandle_t handle, const uint8_t *data, size_t length )
{
	streamConfig_t *config = handle;
	uint8_t status = SYSTEM_STATUS_FAIL;

	if ( config->GetAvailToWrite() == length )
	{
		for(int i = 0; i < length; ++i)
		{
			config->Write( data[i] );
		}
		status = SYSTEM_STATUS_SUCCESS;
	}
	return status;

}

/**********************************************************************************/
size_t Stream_GetAvailToWrite( streamHandle_t handle )
{
	streamConfig_t *config = handle;
	return config->GetAvailToWrite();
}

/**********************************************************************************/
size_t Stream_GetBytesToRead( streamHandle_t handle )
{
	streamConfig_t *config = handle;
	return config->GetBytesToRead();

}
