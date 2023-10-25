#ifndef SYSTEM_COMMON_H_
#define SYSTEM_COMMON_H_

//#define NDEBUG

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <MKL05Z4.h>

#define __OS_PROJECT

// The general status codes for use in the whole system.
enum
{
	SYSTEM_STATUS_SUCCESS = 0,  /*!< Generic status for Success. */
	SYSTEM_STATUS_FAIL = 1,      /*!< Generic status for Fail. */
	SYSTEM_STATUS_READ_ONLY = 2,    /*!< Generic status for read only failure. */
	SYSTEM_STATUS_OUT_OF_RANGE = 3,   /*!< Generic status for out of range access. */
	SYSTEM_STATUS_INVALID_ARGUMENT = 4,   /*!< Generic status for invalid argument check. */
	SYSTEM_STATUS_TIMEOUT = 5,   /*!< Generic status for timeout. */
	SYSTEM_STATUS_NO_TRANSFER_IN_PROGRESS = 6,   /*!< Generic status for no transfer in progress. */
	SYSTEM_STATUS_BUSY = 7,   /*!< Generic status for module is busy. */
	SYSTEM_STATUS_INVALID_ADDRESS = 8,   /*!< Generic status for invalid address. */
	SYSTEM_STATUS_INVALID_REGISTER = 9,   /*!< Generic status for invalid register. */
	SYSTEM_STATUS_TRANSFER_FAIL = 10,   /*!< Generic status for transfer failure. */
};

typedef enum{
	NO_CLK_FREQ			    = 0,
	MCG_FLL_CLK_FREQ	= DEFAULT_SYSTEM_CLOCK,
	OSC_ER_CLK_FREQ		    = CPU_XTAL_CLK_HZ, /*Oscilator External Reference Clock*/
	MCG_IRC_CLK_FREQ	    = CPU_INT_SLOW_CLK_HZ,
	LPO_CLK_FREQ		    = 1000U,
	BUS_CLK_FREQ		    = DEFAULT_SYSTEM_CLOCK,
	ERCLK_32K_FREQ		    = 32768U
}systemClock_t;

#ifdef __OS_PROJECT
#define SYSTEM_MALLOC( x ) OS_Heap_Alloc( x )
#define SYSTEM_FREE( x )
#else
#include <stdlib.h>
#define SYSTEM_MALLOC( x ) malloc( x )
#define SYSTEM_FREE( x ) free( x )
#endif

/*!< If FreeRTOS is present, uncomment this macro or set in the compiler parameters.*/
//#define __FREERTOS_H

//#define NDEBUG

#ifdef NDEBUG
#define SYSTEM_ASSERT(n) (void)0
#else
#ifdef __FREERTOS_H
#include <FreeRTOS.h>
#include <task.h>
#define SYSTEM_ASSERT(n) configASSERT(n)
#else
#define SYSTEM_ASSERT(n) \
    while (!(n))  \
    {             \
                 \
    }
#endif //__FREERTOS_H
#endif // NDEBUG

#endif /* SYSTEM_COMMON_H_ */
