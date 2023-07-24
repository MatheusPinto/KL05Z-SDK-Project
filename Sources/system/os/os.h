/*
 * os.h
 *
 *  Created on: 20/06/2023
 *      Author: mathe
 */

#ifndef SOURCES_SYSTEM_OS_OS_H_
#define SOURCES_SYSTEM_OS_OS_H_

#include <FreeRTOS.h>

#define OS_TIME_INFINITY portMAX_DELAY
#define OS_TICK_RATE_MS portTICK_RATE_MS
#define OS_MINIMAL_STACK_SIZE configMINIMAL_STACK_SIZE

typedef TickType_t osTick_t;

#endif /* SOURCES_SYSTEM_OS_OS_H_ */
