/*
 * task.h
 *
 *  Created on: 20/06/2023
 *      Author: mathe
 */

#ifndef SOURCES_SYSTEM_OS_TASK_H_
#define SOURCES_SYSTEM_OS_TASK_H_

#include "os.h"
#include "../Freertos/task.h"

#define OS_TASKS_NUMBER 10

typedef void (*osTaskFunction_t)( void );

#define OS_TASK_CODE( function ) void function( void )


void OS_Task_Create( osTaskFunction_t code,
		             const char *name,
					 size_t stackLen,
					 size_t prio,
					 size_t period );

static inline void OS_Task_Delay( osTick_t delay )
{
	vTaskDelay( delay );
}


static inline void OS_Task_DelayUntil( osTick_t *const prevTime, osTick_t increment )
{
	vTaskDelayUntil( prevTime, increment );
}

/*
void OS_Task_SignalInit(uint16_t tasksNumber);

void OS_Task_WaitInitSignal(void);

void OS_Task_NotifyInitSignal(void);

size_t OS_Task_GetInitialTime(void);
*/

#endif /* SOURCES_SYSTEM_OS_TASK_H_ */
