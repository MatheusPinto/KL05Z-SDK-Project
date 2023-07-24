/*
 * mutex.h
 *
 *  Created on: 20/06/2023
 *      Author: mathe
 */

#ifndef SOURCES_SYSTEM_OS_MUTEX_H_
#define SOURCES_SYSTEM_OS_MUTEX_H_

#include "os.h"
#include <semphr.h>

typedef SemaphoreHandle_t osMutex_t;

static inline osMutex_t OS_Mutex_Create( void )
{
	return xSemaphoreCreateMutex();
}

static inline size_t OS_Mutex_Take( osMutex_t handle, osTick_t blockTime)
{
	return xSemaphoreTake( handle, blockTime );
}

static inline size_t OS_Mutex_Give( osMutex_t handle )
{
	return xSemaphoreGive( handle );
}




#endif /* SOURCES_SYSTEM_OS_MUTEX_H_ */
