#include "MKL05Z4.h"
#include "system/os/os.h"
#include "system/os/scheduler.h"
#include "system/os/heap.h"
#include "system/os/task.h"
#include "system/os/mutex.h"

osMutex_t mtx;
size_t heapSize;

OS_TASK_CODE(task1)
{
	heapSize = OS_Heap_GetFreeSize();

	OS_Mutex_Take(mtx, OS_TIME_INFINITY);
	GPIOB->PTOR = 1 << 9;
	OS_Mutex_Give(mtx);
}

OS_TASK_CODE(task2)
{
	GPIOB->PTOR = 1 << 8;
}

int main(void)
{
	SIM->SCGC5 |= 1 << 10;
	PORTB->PCR[9] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= 1 << 9;
	PORTB->PCR[8] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= 1 << 8;

	heapSize = OS_Heap_GetFreeSize();

	mtx = OS_Mutex_Create();

	OS_Task_Create( task1,
			        "myTask",
				    configMINIMAL_STACK_SIZE,
				    2,
				    2000);

	OS_Task_Create( task2,
			        "myTask2",
				    configMINIMAL_STACK_SIZE,
				    1,
				    1000);

	OS_Scheduler_SetPolicy( OS_SCHED_RM );
	OS_Scheduler_Start();

	return 0;
}
