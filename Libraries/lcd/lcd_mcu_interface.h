/*
 * lcd_mcu_interface.h
 *
 *  Created on: 10/10/2023
 *      Author: mathe
 */

#ifndef LCD_HD44780_MCU_INTERFACE_H_
#define LCD_HD44780_MCU_INTERFACE_H_

/** Utils */
#include "libraries/delay/delay.h"

#ifdef __FREERTOS_H
#include "FreeRTOS.h"
#include "semphr.h"
#endif /* __FREERTOS_H */


#ifdef __FREERTOS_H
#define _LCD_USE_RTOS
#endif

#define MCU_PortSet(portPinsRegister, portPinMask) portPinsRegister->PSOR = portPinMask;

#define MCU_PortClear(portPinsRegister, portPinMask) portPinsRegister->PCOR = portPinMask;

/* waiting macros */
#define Waitns(x) \
		Delay_Waitns(x)				 /* Wait x ns */
#define Waitus(x) \
		Delay_Waitus(x)				 /* Wait x us */
#define Waitms(x) \
		Delay_Waitms(x)				 /* Wait x ms */

#define LCD_RTOS_ENTER_MUTEX(x) xSemaphoreTake(x->lcdAccessMutex, portMAX_DELAY)
#define LCD_RTOS_EXIT_MUTEX(x) xSemaphoreGive(x->lcdAccessMutex)

#ifdef __FREERTOS_H
typedef xSemaphoreHandle lcdRTOSSemaphore_t;
#endif

#endif /* LCD_HD44780_MCU_INTERFACE_H_ */
