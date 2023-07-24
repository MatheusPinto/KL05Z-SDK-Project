/*
 * task.c
 *
 *  Created on: 21/06/2023
 *      Author: mathe
 */

#include "task.h"
#include "semaphore.h"
#include "scheduler.h"

static uint16_t g_tasksNumber;
static osSemaphore_t g_initSignalSemaphore;
static osTick_t g_tasksInitialTime;

typedef struct
{
	size_t period;
	osTaskFunction_t code;
}osTaskParam_t;

osTaskParam_t appTask[OS_TASKS_NUMBER];
uint8_t appTaskCount;

void genericLoopBody( osTaskParam_t *args )
{
	osTick_t prevTime = OS_Scheduler_GetTickCount();

	for ( ; ; )
	{
		args->code();
		OS_Task_DelayUntil(&prevTime, args->period / portTICK_RATE_MS);
	}
}

void OS_Task_Create( osTaskFunction_t code,
		             const char *name,
					 size_t stackLen,
					 size_t prio,
					 size_t period )
{
	appTask[appTaskCount].code = code;
	appTask[appTaskCount].period = period;

	xTaskCreate( ( TaskFunction_t )genericLoopBody,
			     name,
				 stackLen,
				 &appTask[appTaskCount],
				 prio,
				 NULL );

	++appTaskCount;
}

/*
void OS_Task_SignalInit(uint16_t tasksNumber)
{
	g_tasksNumber = tasksNumber;

	g_initSignalSemaphore = OS_Semaphore_Create( g_tasksNumber );

	SYSTEM_ASSERT(g_initSignalSemaphore);
}

void OS_Task_WaitInitSignal(void)
{
	OS_Semaphore_Take( g_initSignalSemaphore, OS_INFINITY_DELAY );
}


void OS_Task_NotifyInitSignal(void)
{
	for ( int i = 0; i < g_tasksNumber; ++i )
	{
		OS_Semaphore_Give( g_initSignalSemaphore );
	}
	g_tasksInitialTime =  OS_Scheduler_GetTickCount(); // Get the first task activation
}

size_t OS_Task_GetInitialTime(void)
{
	return g_tasksInitialTime;
}
*/
