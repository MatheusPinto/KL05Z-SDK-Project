/*
 * stack_overflow_hook.c
 *
 *  Created on: 13/06/2023
 *      Author: mathe
 */
#include "FreeRTOS.h"
#include "task.h"

void OS_App_StackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // Custom behavior when a stack overflow occurs
    // You can log an error, take corrective actions, etc.
}


