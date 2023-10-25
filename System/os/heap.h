/*
 * heap.h
 *
 *  Created on: 20/06/2023
 *      Author: mathe
 */

#ifndef SOURCES_SYSTEM_OS_HEAP_H_
#define SOURCES_SYSTEM_OS_HEAP_H_

#include "os.h"

static inline void* OS_Heap_Alloc( size_t size)
{
	return pvPortMalloc( size );
}


static inline size_t OS_Heap_GetFreeSize( void )
{
	return xPortGetFreeHeapSize();
}

#endif /* SOURCES_SYSTEM_OS_HEAP_H_ */
