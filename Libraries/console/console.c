/**
 * @file	console.c
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

#include <common.h>
#include <Libraries/printf/printf.h>
#include <Libraries/util/string.h>
#ifdef __OS_PROJECT
#include <System/os/os.h>
#include <System/os/mutex.h>
#endif /* __OS_PROJECT */

#include "console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!< A type for console handle structures. */
struct consoleHandle{
	consoleConfig_t* config; /*!< The console configuration structure. */
#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	osMutex_t consoleAccessMutex; /*!< Protect the console from race condition access. */
#endif /* __OS_PROJECT */
};

#ifdef CONSOLE_STATIC_OBJECTS_CREATION
/*!< A list with all console configuration structures used in application. */
static consoleConfig_t g_consoleConfigList[CONSOLE_MAX_STATIC_OBJECTS];
/*!< A list with all console handle structures used in application. */
static struct consoleHandle g_consoleHandleList[CONSOLE_MAX_STATIC_OBJECTS];
/*!< The number of configuration and handle structures created. */
static uint8_t g_staticConfigsCreated, g_staticHandlesCreated;
#endif


/*******************************************************************************
 * Code
 ******************************************************************************/

static void SendESCPrefix( struct consoleHandle *consoleHandle )
{
	uint8_t escPrefix[] = { 0x1BU, 0x5BU }; // 'ESC' and '[' prefix

	Stream_WriteBlocking( consoleHandle->config->stream, escPrefix, 2 );
}


void outPrintf( char character, void* arg )
{
	struct consoleHandle *handle = arg;

	Stream_WriteBlocking( handle->config->stream, &character, 1 );
}


void Console_PrintNum( consoleHandle_t handle, int32_t num, uint8_t base )
{
	char str[CONSOLE_MAX_NUMBER_BUFFER_LEN];

	Util_IntToStr( num, str, base );
	Console_Print( handle, str );
}


void Console_PrintFloat( consoleHandle_t handle, float num, int afterPoint )
{
	char str[CONSOLE_MAX_NUMBER_BUFFER_LEN];

	Util_FloatToStr( num, str, afterPoint );
	Console_Print( handle, str );
}

/**********************************************************************************/
consoleConfig_t* Console_CreateConfig(void)
{
	consoleConfig_t *ret;

#ifdef	CONSOLE_STATIC_OBJECTS_CREATION
	if ( g_staticConfigsCreated < CONSOLE_MAX_STATIC_OBJECTS )
	{
		ret = &g_consoleConfigList[g_staticConfigsCreated];
	}
	else
	{
		ret = NULL;
	}
#else
	ret = SYSTEM_MALLOC( sizeof ( consoleConfig_t ) );
#endif
	return ret;
}

/**********************************************************************************/
consoleHandle_t Console_Init(consoleConfig_t *config)
{
	SYSTEM_ASSERT( config );

	struct consoleHandle* handle = NULL;

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	osMutex_t mtx;
#endif // __OS_PROJECT

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	mtx = OS_Mutex_Create();
	if( mtx != NULL )
	{
#endif // __OS_PROJECT

#ifdef	CONSOLE_STATIC_OBJECTS_CREATION
			if ( g_staticHandlesCreated < CONSOLE_MAX_STATIC_OBJECTS )
			{
				handle = &g_consoleHandleList[g_staticHandlesCreated];
			}
#else
			handle = SYSTEM_MALLOC( sizeof ( struct consoleHandle ) );
#endif
			if ( handle != NULL)
			{
				handle->config = config;
#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
				handle->consoleAccessMutex = mtx;
#endif // __OS_PROJECT
			}

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	}
#endif // __OS_PROJECT

	return handle;
}

/**********************************************************************************/
void Console_Scan( consoleHandle_t handle, char *str, size_t length )
{
	struct consoleHandle *consoleHandle = handle;

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	OS_Mutex_Take(consoleHandle->consoleAccessMutex, OS_TIME_INFINITY);
#endif

	Stream_ReadBlocking( consoleHandle->config->stream, (uint8_t *)str, length );

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	OS_Mutex_Give(consoleHandle->consoleAccessMutex);
#endif

}

/**********************************************************************************/
void Console_Print(consoleHandle_t handle, const char* str)
{
	struct consoleHandle *consoleHandle = handle;

	size_t i;
	for(i = 0; str[i] != '\0'; ++i)
	{}

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	OS_Mutex_Take(consoleHandle->consoleAccessMutex, OS_TIME_INFINITY);
#endif

	Stream_WriteBlocking( consoleHandle->config->stream, (uint8_t*)str, i );

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	OS_Mutex_Give(consoleHandle->consoleAccessMutex);
#endif
}

/**********************************************************************************/
#ifdef CONSOLE_PRINTLN_FUNCTION
void Console_Scanln(consoleHandle_t handle, char line[])
{
	struct consoleHandle *consoleHandle = handle;

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	OS_Mutex_Take(consoleHandle->consoleAccessMutex, OS_TIME_INFINITY);
#endif

	size_t newLineSize;
	for(newLineSize = 0; consoleHandle->config->newLine[newLineSize] != '\0'; ++newLineSize)
	{}

	for ( int i = 0; ; ++i )
	{
		while( !Stream_GetBytesToRead(consoleHandle->config->stream ) );
		Stream_Read( consoleHandle->config->stream, &line[i], 1 );

		int j;
		for ( j = newLineSize - 1; j > -1; j-- )
		{
			if ( line[i-j] != consoleHandle->config->newLine[newLineSize-1-j] )
			{
				break;
			}
		}
		if ( j < 0 )
		{
			line[i-newLineSize+1] = '\0';
			break;
		}
	}

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	OS_Mutex_Give(consoleHandle->consoleAccessMutex);
#endif

}
#endif

/**********************************************************************************/
#ifdef CONSOLE_PRINTLN_FUNCTION
void Console_Println(consoleHandle_t handle, const char* line)
{
	struct consoleHandle *consoleHandle = handle;

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	OS_Mutex_Take(consoleHandle->consoleAccessMutex, OS_TIME_INFINITY);
#endif

	size_t i;
	for(i = 0; line[i] != '\0'; ++i)
	{}

	Stream_WriteBlocking( consoleHandle->config->stream, (uint8_t*)line, i );

	for ( int i = 0; consoleHandle->config->newLine[i]; ++i )
	{
		while( !Stream_GetAvailToWrite(consoleHandle->config->stream ) );
		Stream_WriteBlocking( consoleHandle->config->stream, (uint8_t *)&consoleHandle->config->newLine[i], 1 );
	}

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	OS_Mutex_Give(consoleHandle->consoleAccessMutex);
#endif

}
#endif

/**********************************************************************************/
char Console_GetChar( consoleHandle_t handle )
{
	struct consoleHandle* consoleHandle = handle;
	char ch;

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	OS_Mutex_Take(consoleHandle->consoleAccessMutex, OS_TIME_INFINITY);
#endif

	Stream_ReadBlocking( consoleHandle->config->stream, (uint8_t *)&ch, 1 );

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	OS_Mutex_Give(consoleHandle->consoleAccessMutex);
#endif
	return ch;
}

/**********************************************************************************/
void Console_PutChar( consoleHandle_t handle, char ch )
{
	struct consoleHandle *consoleHandle = handle;

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	OS_Mutex_Take(consoleHandle->consoleAccessMutex, OS_TIME_INFINITY);
#endif

	Stream_WriteBlocking( consoleHandle->config->stream, (uint8_t*)&ch, 1 );

#if defined(__OS_PROJECT) && defined(CONSOLE_IS_REENTRANT)
	OS_Mutex_Give(consoleHandle->consoleAccessMutex);
#endif
}

/**********************************************************************************/
void Console_Clear( consoleHandle_t handle )
{
	struct consoleHandle *consoleHandle = handle;
	const char scapceSequence[] = { '2', 'J' };

	SendESCPrefix( consoleHandle );
	Stream_WriteBlocking( consoleHandle->config->stream, scapceSequence, 2 );
}

/**********************************************************************************/
void Console_PlayBell( consoleHandle_t handle )
{
	struct consoleHandle *consoleHandle = handle;
	uint8_t seven = 7;
	Stream_WriteBlocking( consoleHandle->config->stream, &seven, 1 );
}

#ifdef CONSOLE_IS_ANSI

/**********************************************************************************/
void Console_SetCursor( consoleHandle_t handle, consoleCursor_t direction, uint8_t intervals )
{
	struct consoleHandle *consoleHandle = handle;

	SendESCPrefix( consoleHandle );
	Console_PrintNum( handle, intervals, 10 );

	char ch = (char)direction;
	Stream_Write( consoleHandle->config->stream, &ch, 1 );
}

/**********************************************************************************/
void Console_MoveTo( consoleHandle_t handle, uint8_t x, uint8_t y )
{
	struct consoleHandle *consoleHandle = handle;

	SendESCPrefix( consoleHandle );
	Console_PrintNum( handle, y, 10 );
	Console_PutChar( handle, ';');
	Console_PrintNum( handle, x, 10 );
	Console_PutChar( handle, 'H');
}

/**********************************************************************************/
void Console_SetCharBold( consoleHandle_t handle )
{
	Console_PutChar( handle , ';');
	Console_PutChar( handle , '1' );
}

/**********************************************************************************/
void Console_SetCharColor(consoleHandle_t handle, consoleColor_t foreground, consoleColor_t background)
{
	Console_PutChar( handle, ';' );
	Console_PrintNum( handle, (int32_t)foreground, 10 );
	Console_PutChar( handle, ';' );
	Console_PrintNum( handle, ((int32_t)background)+10, 10 );
}

/**********************************************************************************/
void Console_SetCharNormal( consoleHandle_t handle )
{
	Console_PutChar( handle , ';');
	Console_PutChar( handle , '0' );
}

/**********************************************************************************/
void Console_SetCharUnderlined( consoleHandle_t handle )
{
	Console_PutChar( handle , ';');
	Console_PutChar( handle , '4' );
}

/**********************************************************************************/
void Console_EnterCharAttrMode( consoleHandle_t handle )
{
	struct consoleHandle *consoleHandle = handle;

	SendESCPrefix( consoleHandle );
	Console_PutChar( handle, '0' );
}

/**********************************************************************************/
void Console_ExitCharAttrMode( consoleHandle_t handle )
{
	Console_PutChar(handle,'m');
}

/**********************************************************************************/
void Console_EraseLine( consoleHandle_t handle )
{
	struct consoleHandle *consoleHandle = handle;

	SendESCPrefix( consoleHandle );
	char ch = 'K';
	Stream_Write(handle, &ch, 1);
}

#endif //CONSOLE_IS_ANSI
