# Delay

A library with delay generation functions.

This library was adapted from Processor Expert Component by Erich Styger:
https://sourceforge.net/projects/mcuoneclipse/files/

Supported CPUs:

- ARM-Cortex M0 to M7;
- RISC-V

Supported OSes:

- FreeRTOS.

To use the Delay functions, follow these steps:

- Set the DELAY_CLOCK_FREQUENCY macro present in the `delay.h` with the specific microcontroller CPU clock frequency.
- Call the `Delay_Init` function before use any other function of the Delay module.


# Definitions

- `DELAY_CPU_IS_ARM_CORTEX_M`: Comment this macro, if an ARM Cortex CPU is being used.
- `DELAY_ARM_CORTEX_M`: The ARM Cortex CPU number.
- `DELAY_CPU_IS_RISC_V`: Macro for RISC-V CPU.
- `DELAY_USE_ARM_CYCLE_COUNTER`: If ARM Cortex CPU is equal or higher than M3, you can use register cycle counter. Comment if do not want to use it.
- `DELAY_CLOCK_FREQUENCY`: Specific microcontroller CPU clock frequency.

# API

The following functions are available:

---

## Delay_Init

Initialize the delayer module.

**Note:**
Always call this function before using any other API function.

---

## Delay_WaitCycles

Block the current firmware execution until the number of core cycles is reached.

**Parameters:**
- `cycles`: The number of core cycles.

---

## Delay_Waitms

Wait for a specified time in milliseconds.

The firmware execution is blocked inside the function.

**Parameters:**
- `ms`: How many milliseconds the function has to wait.

**Note:**
The mean error obtained in ARM Cortex >= M3 CPUs was about +6.8e-4%.

---

## Delay_Waitus

Wait for a specified time in microseconds.

The firmware execution is blocked inside the function.

**Parameters:**
- `us`: How many microseconds the function has to wait.

**Note:**
The mean error obtained in ARM Cortex >= M3 CPUs was very variable (ex.: +0.04%, 0%, -0.007,...)

---

## Delay_Waitns

Wait for a specified time in nanoseconds.

The firmware execution is blocked inside the function.

**Parameters:**
- `ns`: How many nanoseconds the function has to wait.

**Note:**
This function is basically a wrapper for `Delay_Waitus` to express values in ns. It is not accurate for small intervals expressed in ns.

---

## Delay_WaitOSms

If an RTOS is enabled, this routine will use a non-blocking wait method. Otherwise, it will do a busy/blocking wait.

**Parameters:**
- `ms`: How many milliseconds the function has to wait.

---


# Example 

This is a generic blink LED example.

```c
...
#include "libraries/delay/delay.h"

int main( void )
{
	/* Microcontroller specific hardware initilization */
	...
	
	Delay_Init();
	
	/* Blinks a LED every 1 second */
	for ( ; ; )
	{
		Delay_Waitms( 1000 );
		LED_Toggle();

	}
}

```