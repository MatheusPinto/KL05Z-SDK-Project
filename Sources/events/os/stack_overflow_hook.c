#include "Freertos/FreeRTOS.h"
#include "Freertos/task.h"

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // Custom behavior when a stack overflow occurs
    // You can log an error, take corrective actions, etc.
}


