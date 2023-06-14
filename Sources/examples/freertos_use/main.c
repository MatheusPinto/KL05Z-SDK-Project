#include "MKL05Z4.h"
#include "FreeRTOS.h"
#include "task.h"
#include <semphr.h>

SemaphoreHandle_t sem;
size_t heapSize;

portTASK_FUNCTION(task1, arg)
{
	TickType_t prevTime = OS_Task_GetTickCount();

	sem = OS_Semaphore_CreateBinary();

	OS_Semaphore_Give(sem);

	heapSize = OS_Port_GetFreeHeapSize();

	for(;;)
	{
		OS_Semaphore_Take(sem, portMAX_DELAY);
		GPIOB->PTOR = 1 << 9;
		OS_Task_DelayUntil(&prevTime, 1000/portTICK_RATE_MS);
		OS_Semaphore_Give(sem);
	}
}

portTASK_FUNCTION(task2, arg)
{
	TickType_t prevTime = OS_Task_GetTickCount();

	for(;;)
	{
		GPIOB->PTOR = 1 << 8;
		OS_Task_DelayUntil(&prevTime, 2000/portTICK_RATE_MS);
	}
}

int main(void)
{
	SIM->SCGC5 |= 1 << 10;
	PORTB->PCR[9] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= 1 << 9;
	PORTB->PCR[8] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= 1 << 8;

	heapSize = OS_Port_GetFreeHeapSize();

	OS_Task_Create( task1,
			        "myTask",
				    configMINIMAL_STACK_SIZE,
				    NULL,
				    2,
				    NULL);

	OS_Task_Create( task2,
			        "myTask2",
				    configMINIMAL_STACK_SIZE,
				    NULL,
				    1,
				    NULL);


	OS_Task_StartScheduler();

	return 0;
}
