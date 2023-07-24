/*
 * semaphore.h
 *
 *  Created on: 20/06/2023
 *      Author: mathe
 */

#ifndef SOURCES_SYSTEM_OS_SEMAPHORE_H_
#define SOURCES_SYSTEM_OS_SEMAPHORE_H_

#include <system/assert.h>
#include "os.h"
#include <semphr.h>

typedef SemaphoreHandle_t osSemaphore_t;

static inline osSemaphore_t OS_Semaphore_Create( size_t count )
{
	osSemaphore_t semaphore;

	SYSTEM_ASSERT( count );

	if ( count > 1 )
	{
		xSemaphoreCreateCounting( count, 0 );
	}
	else
	{
		xSemaphoreCreateBinary();
	}
	return semaphore;
}

static inline size_t OS_Semaphore_Take( osSemaphore_t handle, osTick_t blockTime)
{
	return xSemaphoreTake( handle, blockTime );
}

static inline size_t OS_Semaphore_Give( osSemaphore_t handle )
{
	return xSemaphoreGive( handle );
}

#endif /* SOURCES_SYSTEM_OS_SEMAPHORE_H_ */
