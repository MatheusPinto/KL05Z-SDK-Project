/*
 * scheduler.h
 *
 *  Created on: 20/06/2023
 *      Author: mathe
 */

#ifndef SOURCES_SYSTEM_OS_SCHEDULER_H_
#define SOURCES_SYSTEM_OS_SCHEDULER_H_

#include "os.h"
#include <task.h>


typedef enum
{
	OS_SCHED_RR = 0,
	OS_SCHED_COLAB,
	OS_SCHED_RM,
	OS_SCHED_EDF
}osSchedulerPolicy_t;

static inline void OS_Scheduler_SetPolicy( osSchedulerPolicy_t policy )
{

}

static inline void OS_Scheduler_Start( void )
{
	vTaskStartScheduler();
}

static inline osTick_t OS_Scheduler_GetTickCount( void )
{
	return xTaskGetTickCount();
}

#endif /* SOURCES_SYSTEM_OS_SCHEDULER_H_ */
