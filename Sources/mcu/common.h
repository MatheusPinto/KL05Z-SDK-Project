/*
 * COMMON_HAL.h
 *
 *  Created on: 19/10/2022
 *      Author: mathe
 */

#ifndef SOURCES_DRIVERS_COMMON_DRIVER_H_
#define SOURCES_DRIVERS_COMMON_DRIVER_H_

//#define NDEBUG

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <MKL05Z4.h>
#include <system/assert.h>
#include <system/status_code.h>


typedef enum{
	NO_CLK_FREQ			= 0,
	MCG_FLL_CLK_FREQ	= DEFAULT_SYSTEM_CLOCK,
	OSC_ER_CLK_FREQ		= CPU_XTAL_CLK_HZ, /*Oscilator External Reference Clock*/
	MCG_IRC_CLK_FREQ	= CPU_INT_SLOW_CLK_HZ,
	LPO_CLK_FREQ		= 1000U,
	BUS_CLK_FREQ		= DEFAULT_SYSTEM_CLOCK,
	ERCLK_32K_FREQ		= 32768U
}mcuClock_t;


#endif /* SOURCES_DRIVERS_COMMON_DRIVER_H_ */
